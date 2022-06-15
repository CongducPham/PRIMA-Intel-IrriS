#!/bin/bash

# Wait for starting
EDGE_STATUS=
while [ "$EDGE_STATUS" != "healthy" ]
do
  EDGE_STATUS=`docker inspect -f {{.State.Health.Status}} waziup.wazigate-edge`
  echo -n "."
  sleep 5
done

logger -t intel-irris-auto-config "Edge container is now active and healthy"

logger -t intel-irris-auto-config "Configuring frequency band"

if [ -f /boot/intel-irris-band.txt ]
then
	BAND=`cat /boot/intel-irris-band.txt`
else
	BAND="eu433"
fi

logger -t intel-irris-auto-config "Configuring for $BAND"
	
cd /home/pi/scripts
./config_band.sh $BAND

cd /home/pi/
logger -t intel-irris-auto-config "Running INTEL-IRRIS WaziGate auto-configuration script"

if [ -f /boot/intel-irris-auto-config.sh ]
then
	logger -t intel-irris-auto-config "/boot/intel-irris-auto-config.sh found"
	
	if [ -f /boot/intel-irris-auto-config.done ]
	then
		logger -t intel-irris-auto-config "auto-configuration have already been performed – skip"
		echo "auto-configuration have already been performed – skip"
		logger -t intel-irris-auto-config "delete /boot/intel-irris-auto-config.done to restart auto-configuration"		
	else		
		logger -t intel-irris-auto-config "running /boot auto-configuration script"
		/boot/intel-irris-auto-config.sh
		logger -t intel-irris-auto-config "create /boot/intel-irris-auto-config.done"
		echo "auto-configuration done" > /boot/intel-irris-auto-config.done
		logger -t intel-irris-auto-config "reboot to take configuration into account"
		# the script can be launched from a terminal, i.e.
		# sudo /home/pi/intel-irris-auto-config.sh
		# it is possible to bypass the reboot by providing an argument
		# sudo /home/pi/intel-irris-auto-config.sh no-reboot
		if [ $# -eq 0 ]
		then
			reboot
		fi		
	fi	
else
	logger -t intel-irris-auto-config "running default auto-configuration script"
	logger -t intel-irris-auto-config "--> nothing to be done in addition to the default configuration"
fi
