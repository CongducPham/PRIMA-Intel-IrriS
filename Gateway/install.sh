#!/bin/bash

./scripts/config_band.sh $1
./scripts/show_band.sh

echo "apt-get update"
sudo apt-get update

echo "update /var/lib/wazigate/setup.sh"
sudo cp wazigate/setup.sh /var/lib/wazigate/

echo "Installing pip"
sudo apt install -y python3-pip

cd oled
./install.sh

cd ..
./scripts/create_full_capacitive_device.sh

##see https://www.raspberryme.com/ajout-dune-horloge-temps-reel-ds3231-au-raspberry-pi/

#echo "adding support for additional DS3231 RTC module"
#sudo apt install -y i2c-tools
#cp /etc/modules ./tmp-etc-modules
#echo "rtc-ds1307" >> ./tmp-modules
#sudo cp ./tmp-etc-modules /etc/modules
#rm ./tmp-etc-modules
#sudo sed -i 's/^exit 0/echo ds1307 0x68 > \/sys\/class\/i2c-adapter\/i2c-1\/new_device\nhwclock -s\nexit 0/g' /etc/rc.local	

