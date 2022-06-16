#!/bin/bash

logger -t intel-irris-auto-config "create-default-capacitive"

cd /home/pi/scripts
#delete the default device
./delete_sensor.sh
#create SOIL-AREA-1 and device with address 26011DAA
./create_full_capacitive_device.sh