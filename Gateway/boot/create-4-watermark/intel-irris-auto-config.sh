#!/bin/bash

logger -t intel-irris-auto-config "create-4-watermark"

cd /home/pi/scripts
#delete the default device
./delete_sensor.sh
#create SOIL-AREA-1 and device with address 26011DB1
./create_full_tensiometer_device_with_dev_addr.sh 1 B1
#create SOIL-AREA-2 and device with address 26011DB2
./create_full_tensiometer_device_with_dev_addr.sh 2 B2
#create SOIL-AREA-3 and device with address 26011DB3
./create_full_tensiometer_device_with_dev_addr.sh 3 B3
#create SOIL-AREA-4 and device with address 26011DB4
./create_full_tensiometer_device_with_dev_addr.sh 4 B4