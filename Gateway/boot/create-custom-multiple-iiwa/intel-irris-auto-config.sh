#!/bin/bash

logger -t intel-irris-auto-config "create-custom-multiple-iiwa"

cd /home/pi

if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    echo "Need a sequence of device type, name, and adress."
    echo "e.g. create-custom-multiple-iiwa....sh C 1 AE WT 8 B4"
    echo "will create 1 capacitive SOIL-AREA-1 with address 26011DAE"
    echo "and 1 watermark with temperature sensor SOIL-AREA-8 with address 26011DB4"
    echo "for a double WM + temperature : 2WT"
    echo "Default-1: (previous) config if any in file /boot/devices.txt"
    echo "Default-2: C 1 AA WT 2 B1"

    #default-1: file exists and is non empty
    FILE=/boot/devices.txt
    if test -f "$FILE"; then
      # echo "$FILE exists."
      if test -s "$FILE"; then
        DEVLIST=`cat $FILE`
        # echo "$FILE non-empty."
        ./boot/create-custom-multiple-iiwa/intel-irris-auto-config.sh $DEVLIST
        exit
      fi
    # default-2
    ./boot/create-custom-multiple-iiwa/intel-irris-auto-config.sh C 1 AA WT 2 B1
    fi
    exit
fi

echo "$@" > /boot/devices.txt

cd /home/pi/scripts

#delete all devices, except gateway devices
echo "--> delete all devices" >> /boot/intel-irris-auto-config.log
./delete_all_devices.sh

#IIWA, first, duplicate the template files
echo "--> copy empty IIWA configuration files from /home/pi/intel-irris-waziapp/config/empty" >> /boot/intel-irris-auto-config.log
cp /home/pi/intel-irris-waziapp/config/empty/*.json .

# iterate over devices
arg_id=0
for var in "$@"
do
  if [ $((arg_id%3)) -eq 0 ]
    then
      DEVTYPE=$var
  elif [ $((arg_id%3)) -eq 1 ]
    then
      DEVID=$var
  elif [ $((arg_id%3)) -eq 2 ]
    then
      DEVADDR=$var
  fi
  arg_id=$((arg_id+1))
  if [ $((arg_id%3)) -eq 0 ]
    then
      if [ $arg_id != 0 ]
        then

          if [ "$DEVTYPE" = "C" ]; then
            #create capacitive SOIL-AREA-1 (i) and device with address 26011DAA, Ab, Ac,...
            echo "--> create_full_capacitive_device_with_dev_addr.sh $DEVID $DEVADDR" >> /boot/intel-irris-auto-config.log
            ./create_full_capacitive_device_with_dev_addr.sh $DEVID $DEVADDR

            #add the voltage monitor sensor
            DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
            echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
            echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log 
            ./create_only_voltage_monitor_sensor.sh $DEVICE

            #IIWA, add first capacitive device id
            echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
            ./add_to_iiwa_devices.sh $DEVICE $DEVID capacitive
            echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
            ./add_to_iiwa_config.sh $DEVICE capacitive

          elif [ "$DEVTYPE" = "CT" ]; then
            #create capacitive SOIL-AREA-1 (i) and device with address 26011DAA, Ab, Ac,...
            echo "--> create_full_capacitive_device_with_dev_addr.sh $DEVID $DEVADDR" >> /boot/intel-irris-auto-config.log
            ./create_full_capacitive_device_with_dev_addr.sh $DEVID $DEVADDR

            DEVICE=`cat /home/pi/scripts/LAST_CREATED_DEVICE.txt`
            echo "--> created device is $DEVICE" >> /boot/intel-irris-auto-config.log
            #add the temperature sensor
            echo "--> calling create_only_temperature_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log
            ./create_only_temperature_sensor.sh $DEVICE
            #add the voltage monitor sensor
            echo "--> calling create_only_voltage_monitor_sensor.sh $DEVICE" >> /boot/intel-irris-auto-config.log 
            ./create_only_voltage_monitor_sensor.sh $DEVICE

            #IIWA, add first capacitive device id
            echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
            ./add_to_iiwa_devices.sh $DEVICE $DEVID capacitive
            echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
            ./add_to_iiwa_config.sh $DEVICE capacitive

          elif [ "$DEVTYPE" = "WT" ]; then
            #create tensiometer SOIL-AREA-2 (after capas) and device with address 26011DB1, b2, b3,...
            echo "--> calling create_full_tensiometer_device_with_dev_addr.sh $DEVID $DEVADDR" >> /boot/intel-irris-auto-config.log
            ./create_full_tensiometer_device_with_dev_addr.sh $DEVID $DEVADDR

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
            ./add_to_iiwa_devices.sh $DEVICE $DEVID tensiometer
            echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
            ./add_to_iiwa_config.sh $DEVICE tensiometer

          elif [ "$DEVTYPE" = "2WT" ]; then
            #create tensiometer SOIL-AREA-2 (after capas) and device with address 26011DB1, b2, b3,...
            echo "--> calling create_full_2-tensiometer_device_with_dev_addr.sh $DEVID $DEVADDR" >> /boot/intel-irris-auto-config.log
            ./create_full_2-tensiometer_device_with_dev_addr.sh $DEVID $DEVADDR

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
            ./add_to_iiwa_devices.sh $DEVICE $DEVID 2tensiometers
            echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
            ./add_to_iiwa_config.sh $DEVICE 2tensiometers

          else
              echo "Device type not recognized, skipping."
          fi
          # echo $DEVID $DEVTYPE $DEVADDR
      fi
  fi
done

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt

#IIWA, finally, copy IIWA config file into /home/pi/intel-irris-waziapp/config/ for backup
echo "--> copy new IIWA configuration files to /home/pi/intel-irris-waziapp/config/ for backup" >> /boot/intel-irris-auto-config.log
cp intel_irris_devices.json intel_irris_sensors_configurations.json /home/pi/intel-irris-waziapp/config/

#IIWA, finally, copy IIWA config file into container
echo "--> copy new IIWA configuration files to IIWA container" >> /boot/intel-irris-auto-config.log
docker cp intel_irris_devices.json waziup.intel-irris-waziapp:/root/src/config
docker cp intel_irris_sensors_configurations.json waziup.intel-irris-waziapp:/root/src/config

echo "--> removing IIWA configuration files" >> /boot/intel-irris-auto-config.log
rm -rf intel_irris_devices.json intel_irris_sensors_configurations.json

