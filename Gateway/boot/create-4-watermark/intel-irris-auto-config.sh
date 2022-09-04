#!/bin/bash

logger -t intel-irris-auto-config "create-4-watermark"

cd /home/pi/scripts

#delete all devices, except gateway devices
./delete_all_devices.sh

#create SOIL-AREA-1 and device with address 26011DB1
./create_full_tensiometer_device_with_dev_addr.sh 1 B1
#add the voltage monitor sensor
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
./create_only_voltage_monitor_sensor.sh $DEVICE

#create SOIL-AREA-2 and device with address 26011DB2
./create_full_tensiometer_device_with_dev_addr.sh 2 B2
#add the voltage monitor sensor
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
./create_only_voltage_monitor_sensor.sh $DEVICE

#create SOIL-AREA-3 and device with address 26011DB3
./create_full_tensiometer_device_with_dev_addr.sh 3 B3
#add the voltage monitor sensor
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
./create_only_voltage_monitor_sensor.sh $DEVICE

#create SOIL-AREA-4 and device with address 26011DB4
./create_full_tensiometer_device_with_dev_addr.sh 4 B4
#add the voltage monitor sensor
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
./create_only_voltage_monitor_sensor.sh $DEVICE

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt