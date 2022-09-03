#!/bin/bash

# Ex: backup_tensiometer_device_sensor_values.sh 62c7c657127dbd00011540a6
# this script backups a tensiometer w/soil temperature device

if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    echo "Need the device id"
    echo "e.g. backup_tensiometer_device_sensor_values.sh 62c7c657127dbd00011540a6"
    exit
fi

echo "--> Get sensor's values from device $1 sensor temperatureSensor_0"
/home/pi/scripts/get_sensor_values.sh $1 temperatureSensor_0

echo "--> Get sensor's values from device $1 sensor temperatureSensor_1"
/home/pi/scripts/get_sensor_values.sh $1 temperatureSensor_1

SUB='not found'

STR=`curl -X GET "http://localhost/devices/$1/sensors/temperatureSensor_5" -H  "accept: application/json"`

if [[ "$STR" == *"$SUB"* ]]; then
	echo "no temperatureSensor_5"
else
	echo "--> Get sensor's values from device $1 sensor temperatureSensor_5"
	/home/pi/scripts/get_sensor_values.sh $1 temperatureSensor_5	
fi

STR=`curl -X GET "http://localhost/devices/$1/sensors/analogInput_6" -H  "accept: application/json"`

if [[ "$STR" == *"$SUB"* ]]; then
	echo "no analogInput_6"
else
	echo "--> Get sensor's values from device $1 sensor analogInput_6"
	/home/pi/scripts/get_sensor_values.sh $1 analogInput_6
fi

echo "Done"