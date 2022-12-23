#!/bin/bash

logger -t intel-irris-auto-config "create-default-capacitive-st"

cd /home/pi/scripts

if [ $# -eq 0 ]
then
#delete all devices, except gateway devices
./delete_all_devices.sh
fi

#create SOIL-AREA-1 and device with address 26011DAA
./create_full_capacitive_device.sh

DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
#add the temperature sensor
./create_only_temperature_sensor.sh $DEVICE
#add the voltage monitor sensor
./create_only_voltage_monitor_sensor.sh $DEVICE

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt