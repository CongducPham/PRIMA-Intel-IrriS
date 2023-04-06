#!/bin/bash

logger -t intel-irris-auto-config "create-default-2-watermark"

cd /home/pi/scripts

if [ $# -eq 0 ]
then
#delete all devices, except gateway devices
echo "--> delete all devices" >> /boot/intel-irris-auto-config.log
./delete_all_devices.sh
fi

#create tensiometer SOIL-AREA-1 and device with address 26011DB1
./create_full_2-tensiometer_device_with_dev_addr.sh 1 B1
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
#add the voltage monitor sensor
echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log 
./create_only_voltage_monitor_sensor.sh $DEVICE

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt