#!/bin/bash

echo "--> Installing Python packages for OLED support"
sudo pip3 install adafruit-circuitpython-ssd1306
sudo apt-get --yes --force-yes install python3-pil

echo "--> Enabling OLED service at boot"
sudo cp intel-irris-oled-service.service.txt /etc/systemd/system/intel-irris-oled-service.service
sudo systemctl enable intel-irris-oled-service.service

echo "Reboot to have the service available"
echo "Test with python oled-service.py"
echo "CTRL-C to exit"

