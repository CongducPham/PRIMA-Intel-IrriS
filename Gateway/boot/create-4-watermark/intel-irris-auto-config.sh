#!/bin/bash

logger -t intel-irris-auto-config "create-4-watermark"

cd /home/pi/scripts

if [ $# -eq 0 ]
then
#delete all devices, except gateway devices
echo "--> delete all devices" >> /boot/intel-irris-auto-config.log
./delete_all_devices.sh
fi

#create SOIL-AREA-1 and device with address 26011DB1
echo "--> calling create_full_tensiometer_device_with_dev_addr.sh 1 B1" >> /boot/intel-irris-auto-config.log
./create_full_tensiometer_device_with_dev_addr.sh 1 B1
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
#add the temperature sensor
echo "--> calling create_only_temperature_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_temperature_sensor.sh $DEVICE
#add the voltage monitor sensor
echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_voltage_monitor_sensor.sh $DEVICE

#create SOIL-AREA-2 and device with address 26011DB2
echo "--> calling create_full_tensiometer_device_with_dev_addr.sh 2 B2" >> /boot/intel-irris-auto-config.log
./create_full_tensiometer_device_with_dev_addr.sh 2 B2
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
#add the temperature sensor
echo "--> calling create_only_temperature_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_temperature_sensor.sh $DEVICE
#add the voltage monitor sensor
echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_voltage_monitor_sensor.sh $DEVICE

#create SOIL-AREA-3 and device with address 26011DB3
echo "--> calling create_full_tensiometer_device_with_dev_addr.sh 3 B3" >> /boot/intel-irris-auto-config.log
./create_full_tensiometer_device_with_dev_addr.sh 3 B3
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
#add the temperature sensor
echo "--> calling create_only_temperature_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_temperature_sensor.sh $DEVICE
#add the voltage monitor sensor
echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_voltage_monitor_sensor.sh $DEVICE

#create SOIL-AREA-4 and device with address 26011DB4
echo "--> calling create_full_tensiometer_device_with_dev_addr.sh 4 B4" >> /boot/intel-irris-auto-config.log
./create_full_tensiometer_device_with_dev_addr.sh 4 B4
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