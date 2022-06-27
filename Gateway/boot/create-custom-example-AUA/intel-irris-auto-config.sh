#!/bin/bash

logger -t intel-irris-auto-config "create-custom-example-AUA"

cd /home/pi/scripts
#delete all sensors
./delete_all_sensors.sh
#create capacitive SOIL-AREA-1 and device with address 26011DAA
./create_full_capacitive_device.sh
#create tensiometer SOIL-AREA-2 and device with address 26011DB2
./create_full_2-tensiometer_device_with_dev_addr.sh 2 B2
#create only the temperature sensor
./create_only_temperature_sensor.sh 2