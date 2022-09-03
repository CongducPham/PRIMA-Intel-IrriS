#!/bin/bash

logger -t intel-irris-auto-config "create-default-watermark"

cd /home/pi/scripts

#delete all sensors
./delete_all_sensors.sh

#create tensiometer SOIL-AREA-1 and device with address 26011DB1
./create_full_tensiometer_device.sh

#add the voltage monitor sensor
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
./create_only_voltage_monitor_sensor.sh $DEVICE

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt