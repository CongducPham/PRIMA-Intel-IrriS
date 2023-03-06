#!/bin/bash

logger -t intel-irris-auto-config "create-starter-kit-demo-capacitive-watermark-st-iiwa-ha"

echo "create-starter-kit-demo-capacitive-watermark-st-iiwa-ha" >> /boot/intel-irris-auto-config.log

cd /home/pi/scripts

if [ $# -eq 0 ]
then
#delete all devices, except gateway devices
echo "--> delete all devices" >> /boot/intel-irris-auto-config.log
./delete_all_devices.sh
fi

#create capacitive SOIL-AREA-1 and device with address 26011DAA
echo "--> calling create_full_capacitive_device_with_dev_addr.sh 1 AA" >> /boot/intel-irris-auto-config.log
./create_full_capacitive_device_with_dev_addr.sh 1 AA

#add the voltage monitor sensor
DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log 
./create_only_voltage_monitor_sensor.sh $DEVICE

#IIWA, first, duplicate the template files
echo "--> copy empty IIWA configuration files from /home/pi/intel-irris-waziapp/config/empty" >> /boot/intel-irris-auto-config.log
cp /home/pi/intel-irris-waziapp/config/empty/*.json .

#IIWA, add first capacitive device id
echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
./add_to_iiwa_devices.sh $DEVICE 1 capacitive
echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
./add_to_iiwa_config.sh $DEVICE capacitive

#and make it the active device
echo "--> make $DEVICE the active device for IIWA" >> /boot/intel-irris-auto-config.log
echo "[]" >> intel-irris-active-device.json
tmpfile=$(mktemp)
jq ". += [{\"device_id\":\"${DEVICE}\",\"sensor_id\":\"temperatureSensor_0\"}]" intel-irris-active-device.json > "$tmpfile" && mv -- "$tmpfile" intel-irris-active-device.json

#HA, first, duplicate the template files
echo "--> copy template HA configuration files from /home/pi/homeassistant" >> /boot/intel-irris-auto-config.log
cd /home/pi/homeassistant
cp configuration_template.yaml configuration.yaml

#HA, replace first capacitive device id
echo "--> add $DEVICE to HA" >> /boot/intel-irris-auto-config.log
sed -i "s/XXX1/$DEVICE/g" configuration.yaml

cd /home/pi/scripts

#create tensiometer SOIL-AREA-2 and device with address 26011DB2
echo "--> calling create_full_tensiometer_device_with_dev_addr.sh 2 B1" >> /boot/intel-irris-auto-config.log
./create_full_tensiometer_device_with_dev_addr.sh 2 B1

DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
#add the temperature sensor
echo "--> calling create_only_temperature_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_temperature_sensor.sh $DEVICE
#add the voltage monitor sensor
echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
./create_only_voltage_monitor_sensor.sh $DEVICE

#IIWA, then add second tensiometer device id
echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
./add_to_iiwa_devices.sh $DEVICE 2 tensiometer
echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
./add_to_iiwa_config.sh $DEVICE tensiometer

#HA, then replace second tensiometer device id
cd /home/pi/homeassistant

echo "--> add $DEVICE to HA" >> /boot/intel-irris-auto-config.log
sed -i "s/XXX2/$DEVICE/g" configuration.yaml

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt

cd /home/pi/scripts

#IIWA, finally, copy IIWA config file into /home/pi/intel-irris-waziapp/config/ for backup
echo "--> copy updated IIWA configuration files to /home/pi/intel-irris-waziapp/config/ for backup"
cp intel_irris_devices.json intel_irris_sensors_configurations.json /home/pi/intel-irris-waziapp/config/

#IIWA, finally, copy IIWA config file into container
echo "--> copy new IIWA configuration files to IIWA container"
docker cp intel_irris_devices.json waziup.intel-irris-waziapp:/root/src/config
docker cp intel_irris_sensors_configurations.json waziup.intel-irris-waziapp:/root/src/config

echo "--> removing configuration files"
rm -rf intel_irris_devices.json intel_irris_sensors_configurations.json intel-irris-conf.json

#HA, finally, copy HA config file into container
cd /home/pi/homeassistant
echo "--> copy new HA configuration files to HA container" >> /boot/intel-irris-auto-config.log
docker cp ./configuration.yaml homeassistant:/config



