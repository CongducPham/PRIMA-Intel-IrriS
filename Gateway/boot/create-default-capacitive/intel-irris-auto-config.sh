#!/bin/bash

logger -t intel-irris-auto-config "create-default-capacitive"

cd /home/pi/scripts

#delete all sensors
./delete_all_sensors.sh

#create SOIL-AREA-1 and device with address 26011DAA
./create_full_capacitive_device.sh

#add the voltage monitor sensor
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
./create_only_voltage_monitor_sensor.sh $DEVICE