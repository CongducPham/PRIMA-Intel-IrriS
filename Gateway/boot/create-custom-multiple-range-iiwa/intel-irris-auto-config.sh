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

#IIWA, first, duplicate the template files
echo "--> copy empty IIWA configuration files from /home/pi/intel-irris-waziapp/config/empty" >> /boot/intel-irris-auto-config.log
cp /home/pi/intel-irris-waziapp/config/empty/*.json .

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

  #IIWA, add first capacitive device id
  echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
  ./add_to_iiwa_devices.sh $DEVICE $i capacitive
  echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
  ./add_to_iiwa_config.sh $DEVICE capacitive

  #and make it the active device
  echo "--> make $DEVICE the active device for IIWA" >> /boot/intel-irris-auto-config.log
  echo "[]" >> intel_irris_active_device.json
  tmpfile=$(mktemp)
  jq ". += [{\"device_id\":\"${DEVICE}\",\"sensor_id\":\"temperatureSensor_0\"}]" intel_irris_active_device.json > "$tmpfile" && mv -- "$tmpfile" intel_irris_active_device.json
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

  #IIWA, then add second tensiometer device id
  echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
  ./add_to_iiwa_devices.sh $DEVICE $(($1 + $i)) tensiometer
  echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
  ./add_to_iiwa_config.sh $DEVICE tensiometer

done

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt

#IIWA, finally, copy IIWA config file into /home/pi/intel-irris-waziapp/config/ for backup
echo "--> copy new IIWA configuration files to /home/pi/intel-irris-waziapp/config/ for backup" >> /boot/intel-irris-auto-config.log
cp intel_irris_devices.json intel_irris_active_device.json intel_irris_sensors_configurations.json /home/pi/intel-irris-waziapp/config/

#IIWA, finally, copy IIWA config file into container
echo "--> copy new IIWA configuration files to IIWA container" >> /boot/intel-irris-auto-config.log
docker cp intel_irris_devices.json waziup.intel-irris-waziapp:/root/src/config
docker cp intel_irris_active_device.json waziup.intel-irris-waziapp:/root/src/config
docker cp intel_irris_sensors_configurations.json waziup.intel-irris-waziapp:/root/src/config

echo "--> removing IIWA configuration files" >> /boot/intel-irris-auto-config.log
rm -rf intel_irris_devices.json intel_irris_active_device.json intel_irris_sensors_configurations.json




