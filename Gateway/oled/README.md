This is a breakdown of all the steps to install the INTEL-IRRIS OLED service on top of the generic WaziGate distribution. The `install.sh` script actually does the installation. 


Expand filesystem
-----------------

	> sudo raspi-config
	
PIP3 on WaziGate
----------------

	> sudo apt install python3-pip
	
OLED on WaziGate
----------------

info from https://learn.adafruit.com/monochrome-oled-breakouts/python-setup

	> sudo pip3 install adafruit-circuitpython-ssd1306
	> sudo apt-get install python3-pil
	
then see small example from https://learn.adafruit.com/monochrome-oled-breakouts/python-usage-2
	
	
The INTEL-IRRIS OLED service
----------------------------

	> sudo cp intel-irris-oled-service.service.txt /etc/systemd/system/intel-irris-oled-service.service
	> sudo systemctl enable intel-irris-oled-service.service
	
Changing the device id
----------------------

edit `key_device.py` and change `device_id_for_oled`.		 