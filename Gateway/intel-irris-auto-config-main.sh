#!/bin/bash

# Wait for WaziGate to be started
# Is service active? = active and is wazigate-system docker container is healthy 
#SYSTEM_STATUS="unhealthy"
#while [ "$(systemctl is-active wazigate)" != "active" ] && [ "$(SYSTEM_STATUS)" != "healthy" ];
#do
#  SYSTEM_STATUS=`docker inspect -f {{.State.Health.Status}} waziup.wazigate-system`
#  sleep 5
#done

# Wait for starting
#EDGE_STATUS=`docker inspect -f {{.State.Health.Status}} waziup.wazigate-system`
#
#while [ "$EDGE_STATUS" != "healthy" ]
#do
#  echo -n "."
#  sleep 5
#  EDGE_STATUS=`docker inspect -f {{.State.Health.Status}} waziup.wazigate-system`
#done

cd /home/pi/
echo `date` >> /boot/intel-irris-auto-config.log 
echo "Running INTEL-IRRIS WaziGate auto-configuration script" >> /boot/intel-irris-auto-config.log

wget -q --spider http://google.com

if [ $? -eq 0 ]; then
        echo "Online" >> /boot/intel-irris-auto-config.log
else
        echo "Offline" >> /boot/intel-irris-auto-config.log
        echo "Get time from RTC" >> /boot/intel-irris-auto-config.log
        hwclock -s
fi

if [ -f /boot/intel-irris-auto-config.done ]
then
	echo "detected previous auto-configuration – skip" >> /boot/intel-irris-auto-config.log
	echo "delete /boot/intel-irris-auto-config.done to restart auto-configuration" >> /boot/intel-irris-auto-config.log
	echo "-----------------------------------------------------------------------" >> /boot/intel-irris-auto-config.log
else

	echo "Looking for frequency band" >> /boot/intel-irris-auto-config.log
	if [ -f /boot/intel-irris-band.txt ]
	then
		cd /home/pi/scripts	
		BAND=`cat /boot/intel-irris-band.txt`
		echo "Configuring for $BAND" >> /boot/intel-irris-auto-config.log
		./config_band.sh $BAND
		echo "auto-configuration for frequency band done" >> /boot/intel-irris-auto-config.done	
		REBOOT="yes"
	else
		echo "keep default frequency band" >> /boot/intel-irris-auto-config.log
	fi

	echo "Looking for /boot/intel-irris-auto-config.sh"	>> /boot/intel-irris-auto-config.log
	if [ -f /boot/intel-irris-auto-config.sh ]
	then
		echo "/boot/intel-irris-auto-config.sh found"	>> /boot/intel-irris-auto-config.log		
		echo "running /boot auto-configuration script" >> /boot/intel-irris-auto-config.log
		/boot/intel-irris-auto-config.sh
		
		echo "auto-configuration for device/sensor done" >> /boot/intel-irris-auto-config.done
		#finally we do not need to reboot when we create the new device
		#REBOOT="yes"
	else
		echo "no /boot/intel-irris-auto-config.sh found" >> /boot/intel-irris-auto-config.log
		echo "nothing to be done in addition to the default configuration" >> /boot/intel-irris-auto-config.log
	fi
		
	if [ "$REBOOT" = "yes" ]
	then
		# the script can be launched from a terminal, i.e.
		# sudo /home/pi/intel-irris-auto-config.sh
		# it is possible to bypass the reboot by providing an argument
		# sudo /home/pi/intel-irris-auto-config.sh no-reboot
		if [ $# -eq 0 ]
		then
			echo "WaziGate will reboot to take auto-config into account" >> /boot/intel-irris-auto-config.log		
			echo "next log will show detected previous auto-configuration – skip" >> /boot/intel-irris-auto-config.log
			echo "-----------------------------------------------------------------------" >> /boot/intel-irris-auto-config.log
			reboot
		else
			echo "should reboot here but script is forced to not reboot"	
		fi
	fi
	echo "-----------------------------------------------------------------------" >> /boot/intel-irris-auto-config.log			
fi