#!/bin/bash

logger -t intel-irris-auto-config "create-custom-multiple-range-iiwa"

if [ $# -eq 0 ]
  then
  	FILE=/home/pi/boot/watermarks.txt
		if test -f "$FILE"; 
			then
				NWMS=`cat $FILE`
				NCAPAS=`cat /home/pi/boot/capas.txt`
		    # echo "$FILE exists."
		    ./boot/create-custom-multiple-range-iiwa/intel-irris-auto-config.sh $NCAPAS $NWMS
	    else
	    	./boot/create-custom-multiple-range-iiwa/intel-irris-auto-config.sh 2 2
		fi
    # echo "No arguments supplied"
    # echo "Need the amount of capacitive devices, and the amount of WM-st devices"
    # echo "e.g. sudo ./boot/create-custom-multiple-range-iiwa/intel-irris-auto-config.sh 3 9"
    # echo "this will create 3 capacitive and 9 WM-st"
    exit
  # else
fi

echo $1 > /home/pi/boot/capas.txt
echo $2 > /home/pi/boot/watermarks.txt


cd /home/pi/scripts

#delete all devices, except gateway devices
echo "--> delete all devices" >> /boot/intel-irris-auto-config.log
./delete_all_devices.sh

#name up to 6 capacitive and 12 WM (could be more, but would need more discussion)
declare -a capas=("AA" "AB" "AC" "AD" "AE" "AF")
declare -a wms=("B1" "B2" "B3" "B4" "B5" "B6" "B7" "B8" "B9" "BA" "BB" "BC")

#IIWA
echo "[{\"device_id\": \"default\", \"device_name\": \"default\"}, " > /home/pi/intel-irris-waziapp/config/intel-irris-devices.json
echo "{\"globals\": {\"soil_salinity\": \"disabled\", \"soil_bulk_density\": \"disabled\"}, \"sensors\": [" > /home/pi/intel-irris-waziapp/config/intel-irris-conf.json


# iterate over capacitive devices
for ((i=1;i<=$1;i++))
do
	# cd /home/pi/scripts

	#create capacitive SOIL-AREA-1 (i) and device with address 26011DAA, Ab, Ac,...
	echo "--> create_full_capacitive_device_with_dev_addr.sh $i ${capas[$i-1]}" >> /boot/intel-irris-auto-config.log
	./create_full_capacitive_device_with_dev_addr.sh $i ${capas[$i-1]}

	#add the voltage monitor sensor
	DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
	echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
	echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log 
	./create_only_voltage_monitor_sensor.sh $DEVICE

	#replace first capacitive device id
	echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
	echo "{\"device_id\": \"$DEVICE\", \"device_name\": \"SOIL-AREA-$i\", \"sensors_structure\": \"1_capacitive\"}," >> /home/pi/intel-irris-waziapp/config/intel-irris-devices.json

	echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
	cp /home/pi/IIWA-templates/IIWA-capa.json IIWA-temp.json
	sed -i "s/XXX1/$DEVICE/g" IIWA-temp.json
	cat IIWA-temp.json >> /home/pi/intel-irris-waziapp/config/intel-irris-conf.json

	#and make it the active device
	echo "--> make $DEVICE the active device for IIWA" >> /boot/intel-irris-auto-config.log
	echo "[{\"device_id\": \"$DEVICE\", \"sensor_id\": \"temperatureSensor_0\"}]" >  /home/pi/intel-irris-waziapp/config/intel-irris-active-device.json
done

# iterate over WM-st devices
for ((i=1;i<=$2;i++))
do
	# cd /home/pi/scripts

	#create tensiometer SOIL-AREA-2 (after capas) and device with address 26011DB1, b2, b3,...
	echo "--> calling create_full_tensiometer_device_with_dev_addr.sh $(($1 + $i)) ${wms[$i-1]}" >> /boot/intel-irris-auto-config.log
	./create_full_tensiometer_device_with_dev_addr.sh $(($1 + $i)) ${wms[$i-1]}

	DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
	echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
	#add the temperature sensor
	echo "--> calling create_only_temperature_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
	./create_only_temperature_sensor.sh $DEVICE
	#add the voltage monitor sensor
	echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
	./create_only_voltage_monitor_sensor.sh $DEVICE

	#then replace second tensiometer device id
	# cd /home/pi/intel-irris-waziapp/config

	echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
	echo "{\"device_id\": \"$DEVICE\", \"device_name\": \"SOIL-AREA-$(($1 + $i))\", \"sensors_structure\": \"1_watermark\"}," >> /home/pi/intel-irris-waziapp/config/intel-irris-devices.json

	echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
	cp /home/pi/IIWA-templates/IIWA-wm-st.json IIWA-temp.json
	sed -i "s/XXX2/$DEVICE/g" IIWA-temp.json
	cat IIWA-temp.json >> /home/pi/intel-irris-waziapp/config/intel-irris-conf.json

done

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt

sed -i "$ s/.$//" /home/pi/intel-irris-waziapp/config/intel-irris-conf.json
sed -i "$ s/.$//" /home/pi/intel-irris-waziapp/config/intel-irris-devices.json

echo "]}" >> /home/pi/intel-irris-waziapp/config/intel-irris-conf.json
echo "]" >> /home/pi/intel-irris-waziapp/config/intel-irris-devices.json

#finally, copy IIWA config file into container
echo "--> copy new IIWA configuration files to IIWA container" >> /boot/intel-irris-auto-config.log
docker cp /home/pi/intel-irris-waziapp/config/intel-irris-devices.json waziup.intel-irris-waziapp:/root/src/config
docker cp /home/pi/intel-irris-waziapp/config/intel-irris-active-device.json waziup.intel-irris-waziapp:/root/src/config
docker cp /home/pi/intel-irris-waziapp/config/intel-irris-conf.json waziup.intel-irris-waziapp:/root/src/config


