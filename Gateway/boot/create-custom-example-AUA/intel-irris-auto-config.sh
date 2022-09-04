#!/bin/bash

logger -t intel-irris-auto-config "create-custom-example-AUA"

cd /home/pi/scripts

#delete all devices, except gateway devices
./delete_all_devices.sh

#create capacitive SOIL-AREA-1 and device with address 26011DAA
./create_full_capacitive_device.sh

#add the voltage monitor sensor
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
./create_only_voltage_monitor_sensor.sh $DEVICE

#create tensiometer SOIL-AREA-2 and device with address 26011DB2
./create_full_2-tensiometer_device_with_dev_addr.sh 2 B2

DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
#add the temperature sensor
./create_only_temperature_sensor.sh $DEVICE
#add the voltage monitor sensor
./create_only_voltage_monitor_sensor.sh $DEVICE

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt