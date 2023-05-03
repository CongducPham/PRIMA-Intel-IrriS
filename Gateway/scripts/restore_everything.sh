#!/bin/bash

cd /home/pi/sensor-backup

/home/pi/scripts/delete_all_devices.sh

capas=$(ls | grep split | grep capa | awk -F'[_.]' '{print $1}' | uniq)
# echo $capas
for k in $capas
do
	echo $k

	devname=$(cat sensor-backup.log | grep $k | grep backup | awk -F'[ ]' '{print $6}' | awk -F'[-]' '{print $3}')
	devaddr=$(cat sensor-backup.log | grep $k | grep backup | awk -F'[ ]' '{print $8}')

	/home/pi/scripts/restore_capacitive_device_sensor_values.sh $devname $devaddr $k $k
	/home/pi/scripts/iiwa_rest.sh add $k SOIL-AREA-$devname 1_capacitive temperatureSensor_0

done

tensios=$(ls | grep split | grep tensio | awk -F'[_.]' '{print $1}' | uniq)
for k in $tensios
do
	echo $k
	devname=$(cat sensor-backup.log | grep $k | grep backup | awk -F'[ ]' '{print $6}' | awk -F'[-]' '{print $3}')
	devaddr=$(cat sensor-backup.log | grep $k | grep backup | awk -F'[ ]' '{print $8}')

	is2WT=$(ls | grep $k | grep "ensor\_2" | wc -l)
	if [ $is2WT -eq 0 ]
	then
		# echo "single"
		/home/pi/scripts/restore_tensiometer_device_sensor_values.sh $devname $devaddr $k $k
		/home/pi/scripts/iiwa_rest.sh add $k SOIL-AREA-$devname 1_watermark temperatureSensor_0
	else
		# echo "2WT"
		/home/pi/scripts/restore_2-tensiometer_device_sensor_values.sh $devname $devaddr $k $k
		/home/pi/scripts/iiwa_rest.sh add $k SOIL-AREA-$devname 2_watermark temperatureSensor_0
		/home/pi/scripts/iiwa_rest.sh add $k SOIL-AREA-$devname 2_watermark temperatureSensor_2
	fi
done
