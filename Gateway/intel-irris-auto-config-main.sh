#!/bin/bash

# Wait for starting
EDGE_STATUS=`docker inspect -f {{.State.Health.Status}} waziup.wazigate-edge`

while [ "$EDGE_STATUS" != "healthy" ]
do
  echo -n "."
  sleep 5
  EDGE_STATUS=`docker inspect -f {{.State.Health.Status}} waziup.wazigate-edge`
done

logger -t intel-irris-auto-config "Edge container is now active and healthy"

cd /home/pi/
logger -t intel-irris-auto-config "Running INTEL-IRRIS WaziGate auto-configuration script"

if [ -f /boot/intel-irris-auto-config.done ]
then
	logger -t intel-irris-auto-config "detected previous auto-configuration – skip"
	echo "detected previous auto-configuration – skip"
	logger -t intel-irris-auto-config "delete /boot/intel-irris-auto-config.done to restart auto-configuration"
	echo "delete /boot/intel-irris-auto-config.done to restart auto-configuration"		
else

	logger -t intel-irris-auto-config "Looking for frequency band"
	if [ -f /boot/intel-irris-band.txt ]
	then
		cd /home/pi/scripts	
		BAND=`cat /boot/intel-irris-band.txt`
		logger -t intel-irris-auto-config "Configuring for $BAND"
		./config_band.sh $BAND
		logger -t intel-irris-auto-config "create /boot/intel-irris-auto-config.done"
		echo "auto-configuration done" > /boot/intel-irris-auto-config.done
		logger -t intel-irris-auto-config "reboot to take configuration into account"		
		REBOOT="yes"
	else
		logger -t intel-irris-auto-config "Keep default frequency band"
		echo "keep default frequency band"
	fi

	logger -t intel-irris-auto-config "Looking for /boot/intel-irris-auto-config.sh"	
	if [ -f /boot/intel-irris-auto-config.sh ]
	then
		logger -t intel-irris-auto-config "/boot/intel-irris-auto-config.sh found"			
		logger -t intel-irris-auto-config "running /boot auto-configuration script"
		/boot/intel-irris-auto-config.sh
		
		logger -t intel-irris-auto-config "create /boot/intel-irris-auto-config.done"
		echo "auto-configuration done" > /boot/intel-irris-auto-config.done
		logger -t intel-irris-auto-config "reboot to take configuration into account"
		REBOOT="yes"
	else
		logger -t intel-irris-auto-config "no /boot/intel-irris-auto-config.sh found"
		logger -t intel-irris-auto-config "nothing to be done in addition to the default configuration"
		echo "nothing to be done in addition to the default configuration"
	fi
	
	if [ "$REBOOT" = "yes" ]
	then
		# the script can be launched from a terminal, i.e.
		# sudo /home/pi/intel-irris-auto-config.sh
		# it is possible to bypass the reboot by providing an argument
		# sudo /home/pi/intel-irris-auto-config.sh no-reboot
		if [ $# -eq 0 ]
		then
			reboot
		else
			echo "should reboot here but script is forced to not reboot"	
		fi
	fi			
fi