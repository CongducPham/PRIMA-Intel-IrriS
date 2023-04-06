#!/bin/bash

logger -t intel-irris-auto-config "create-default-watermark-st"

cd /home/pi/scripts

if [ $# -eq 0 ]
then
#delete all devices, except gateway devices
echo "--> delete all devices" >> /boot/intel-irris-auto-config.log
./delete_all_devices.sh
fi

#create tensiometer SOIL-AREA-1 and device with address 26011DB1
echo "--> calling create_full_tensiometer_device.sh" >> /boot/intel-irris-auto-config.log
./create_full_tensiometer_device.sh
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
#add the temperature sensor
echo "--> calling create_only_temperature_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_temperature_sensor.sh $DEVICE
#add the voltage monitor sensor
echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_voltage_monitor_sensor.sh $DEVICE

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt