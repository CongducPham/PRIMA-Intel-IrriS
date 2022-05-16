#!/bin/bash

./config_band.sh $1
./show_band.sh

echo "apt-get update"
sudo apt-get update

echo "update /var/lib/wazigate/setup.sh"
sudo cp wazigate/setup.sh /var/lib/wazigate/

echo "Installing pip"
sudo apt install -y python3-pip

cd oled
./install.sh

cd ..
./create_full_capacitive_device.sh

