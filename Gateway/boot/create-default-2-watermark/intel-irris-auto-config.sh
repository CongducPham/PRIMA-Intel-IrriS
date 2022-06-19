#!/bin/bash

logger -t intel-irris-auto-config "create-default-2-watermark"

cd /home/pi/scripts
#delete the default device
./delete_sensor.sh
#create tensiometer SOIL-AREA-1 and device with address 26011DB1
./create_full_2-tensiometer_device_with_dev_addr.sh 1 B1