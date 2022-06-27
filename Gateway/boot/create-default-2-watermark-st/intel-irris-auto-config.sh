#!/bin/bash

logger -t intel-irris-auto-config "create-default-2-watermark-st"

cd /home/pi/scripts
#delete all sensors
./delete_all_sensors.sh
#create tensiometer SOIL-AREA-1 and device with address 26011DB1
./create_full_2-tensiometer_device_with_dev_addr.sh 1 B1

DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
#create only the temperature sensor
./create_only_temperature_sensor.sh $DEVICE