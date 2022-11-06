#!/bin/bash

echo "apt-get update"
sudo apt-get update

echo "configure for frequency band $1"
./scripts/config_band.sh $1
./scripts/show_band.sh

if [ -f /var/lib/wazigate/setup.sh ]
then
	#WaziGate v1
	echo "update /var/lib/wazigate/setup.sh"
	sudo cp wazigate/setup.sh /var/lib/wazigate/
else
	#WaziGate v2
	echo "Enabling auto-config service at boot"
	sudo cp intel-irris-auto-config-service.service.txt /etc/systemd/system/intel-irris-auto-config-service.service
	sudo systemctl enable intel-irris-auto-config-service.service	
fi

echo "Installing pip"
sudo apt install -y python3-pip

echo "Installing additional packages"

echo "Installing jq"
sudo apt install -y jq

cd oled
./install.sh

#we are now using the auto-config mechanism
#cd ..
#./boot/create-default-capacitive/intel-irris-auto-config.sh

##see https://www.raspberryme.com/ajout-dune-horloge-temps-reel-ds3231-au-raspberry-pi/
echo "adding support for additional DS3231 RTC module"

sudo apt install -y i2c-tools

#current procedure
cp /etc/modules ./tmp-etc-modules
echo "rtc-ds1307" >> ./tmp-etc-modules
sudo cp ./tmp-etc-modules /etc/modules
rm ./tmp-etc-modules
sudo sed -i 's/^exit 0/echo ds1307 0x68 > \/sys\/class\/i2c-adapter\/i2c-1\/new_device\nhwclock -s\nexit 0/g' /etc/rc.local	

#new procedure
#to https://learn.adafruit.com/adding-a-real-time-clock-to-raspberry-pi/set-rtc-time
#cp /boot/config ./tmp-boot-config
#echo "dtoverlay=i2c-rtc,ds3231" >> ./tmp-boot-config
#sudo cp ./tmp-boot-config /boot/config.txt
#rm ./tmp-boot-config
#sudo apt-get -y remove fake-hwclock
#sudo update-rc.d -f fake-hwclock remove
#sudo systemctl disable fake-hwclock


