#!/bin/bash

cd /home/pi/scripts
./delete_sensor.sh
./create_full_tensiometer_device_with_dev_addr.sh 1 B1
./create_full_tensiometer_device_with_dev_addr.sh 2 B2
./create_full_tensiometer_device_with_dev_addr.sh 3 B3
./create_full_tensiometer_device_with_dev_addr.sh 4 B4