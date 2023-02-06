#!/bin/bash

#only if no argument is provided
if [ $# -eq 0 ]
then
	echo "apt-get update"
	sudo apt-get update

	echo "Installing pip"
	sudo apt install -y python3-pip

	echo "Installing additional packages"

	echo "Installing jq"
	sudo apt install -y jq

	##see https://www.raspberryme.com/ajout-dune-horloge-temps-reel-ds3231-au-raspberry-pi/
	echo "adding support for additional DS3231 RTC module"

	sudo apt install -y i2c-tools

	echo "configure for frequency band $1"
	./scripts/config_band.sh $1
	./scripts/show_band.sh

	echo "configure support for RTC"
	#current procedure
	ds1307=`grep rtc-ds1307 /etc/modules`
	if [ "$ds1307" = "" ]
		then	
			cp /etc/modules ./tmp-etc-modules
			echo "rtc-ds1307" >> ./tmp-etc-modules
			sudo cp ./tmp-etc-modules /etc/modules
			rm ./tmp-etc-modules
		else
			echo "rtc-ds1307 already found in /etc/modules"	
	fi
	
	ds1307=`grep ds1307 /etc/rc.local`
	if [ "$ds1307" = "" ]
		then			
			sudo sed -i 's/^exit 0/echo ds1307 0x68 > \/sys\/class\/i2c-adapter\/i2c-1\/new_device\nhwclock -s\nexit 0/g' /etc/rc.local
		else
			echo "ds1307 already found in /etc/rc.local"	
	fi
	
	#new procedure
	#to https://learn.adafruit.com/adding-a-real-time-clock-to-raspberry-pi/set-rtc-time
	#cp /boot/config ./tmp-boot-config
	#echo "dtoverlay=i2c-rtc,ds3231" >> ./tmp-boot-config
	#sudo cp ./tmp-boot-config /boot/config.txt
	#rm ./tmp-boot-config
	#sudo apt-get -y remove fake-hwclock
	#sudo update-rc.d -f fake-hwclock remove
	#sudo systemctl disable fake-hwclock

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

	cd oled
	./install.sh
fi

cd /home/pi

echo "default crontab is"
cat scripts/crontab.backup
echo "Programming crontab for sensor backup on USB drive"
crontab scripts/crontab.backup
echo "checking with crontab -l"
crontab -l







