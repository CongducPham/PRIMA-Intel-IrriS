#!/bin/bash

logger -t intel-irris-auto-config "create-custom-example"

cd /home/pi/scripts
#delete the default device
./delete_sensor.sh
#create capacitive SOIL-AREA-1 and device with address 26011DAA
./create_full_capacitive_device_with_dev_addr.sh 1 AA
#create tensiometer SOIL-AREA-2 and device with address 26011DB2
./create_full_tensiometer_device_with_dev_addr.sh 2 B2