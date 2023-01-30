#!/bin/bash

logger -t intel-irris-auto-config "create-custom-multiple-iiwa"

if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    echo "Need a sequence of device type, name, and adress."
    echo "e.g. create-custom-multiple-iiwa....sh C 1 AE WT 8 B4"
    echo "will create 1 capacitive SOIL-AREA-1 with address 26011DAE"
    echo "and 1 watermark with temperature sensor SOIL-AREA-8 with address 26011DB4"
    echo "Default-1: (previous) config if any in file /home/pi/boot/devices.txt"
    echo "Default-2: C 1 AA WT 2 B1"

    #default-1: file exists and is non empty
    FILE=/home/pi/boot/devices.txt
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

echo "$@" > /home/pi/boot/devices.txt

cd /home/pi/scripts

#delete all devices, except gateway devices
echo "--> delete all devices" >> /boot/intel-irris-auto-config.log
./delete_all_devices.sh

#IIWA
echo "[{\"device_id\": \"default\", \"device_name\": \"default\"}, " > /home/pi/intel-irris-waziapp/config/intel-irris-devices.json
echo "{\"globals\": {\"soil_salinity\": \"disabled\", \"soil_bulk_density\": \"disabled\"}, \"sensors\": [" > /home/pi/intel-irris-waziapp/config/intel-irris-conf.json


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

            #replace first capacitive device id
            echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
            echo "{\"device_id\": \"$DEVICE\", \"device_name\": \"SOIL-AREA-$DEVID\", \"sensors_structure\": \"1_capacitive\"}," >> /home/pi/intel-irris-waziapp/config/intel-irris-devices.json

            echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
            cp /home/pi/IIWA-templates/IIWA-capa.json IIWA-temp.json
            sed -i "s/XXX1/$DEVICE/g" IIWA-temp.json
            cat IIWA-temp.json >> /home/pi/intel-irris-waziapp/config/intel-irris-conf.json

            #and make it the active device
            echo "--> make $DEVICE the active device for IIWA" >> /boot/intel-irris-auto-config.log
            echo "[{\"device_id\": \"$DEVICE\", \"sensor_id\": \"temperatureSensor_0\"}]" >  /home/pi/intel-irris-waziapp/config/intel-irris-active-device.json

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

            echo "--> add $DEVICE to IIWA" >> /boot/intel-irris-auto-config.log
            echo "{\"device_id\": \"$DEVICE\", \"device_name\": \"SOIL-AREA-$DEVID\", \"sensors_structure\": \"1_watermark\"}," >> /home/pi/intel-irris-waziapp/config/intel-irris-devices.json

            echo "--> set default configuration for $DEVICE in IIWA" >> /boot/intel-irris-auto-config.log
            cp /home/pi/IIWA-templates/IIWA-wm-st.json IIWA-temp.json
            sed -i "s/XXX2/$DEVICE/g" IIWA-temp.json
            cat IIWA-temp.json >> /home/pi/intel-irris-waziapp/config/intel-irris-conf.json
          else
              echo "Device type not recognized, skipping."
          fi
          # echo $DEVID $DEVTYPE $DEVADDR
      fi
  fi
done

#remove LAST_CREATED_DEVICE.txt
rm /home/pi/scripts/LAST_CREATED_DEVICE.txt

# remove ',' at end of enumeration
sed -i "$ s/.$//" /home/pi/intel-irris-waziapp/config/intel-irris-conf.json
sed -i "$ s/.$//" /home/pi/intel-irris-waziapp/config/intel-irris-devices.json

# close lists
echo "]}" >> /home/pi/intel-irris-waziapp/config/intel-irris-conf.json
echo "]" >> /home/pi/intel-irris-waziapp/config/intel-irris-devices.json

#finally, copy IIWA config file into container
echo "--> copy new IIWA configuration files to IIWA container" >> /boot/intel-irris-auto-config.log
docker cp /home/pi/intel-irris-waziapp/config/intel-irris-devices.json waziup.intel-irris-waziapp:/root/src/config
docker cp /home/pi/intel-irris-waziapp/config/intel-irris-active-device.json waziup.intel-irris-waziapp:/root/src/config
docker cp /home/pi/intel-irris-waziapp/config/intel-irris-conf.json waziup.intel-irris-waziapp:/root/src/config


