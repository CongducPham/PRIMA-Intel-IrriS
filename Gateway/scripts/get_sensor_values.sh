#!/bin/bash

# Ex: get_sensor_values.sh 62c7c657127dbd00011540a6 temperatureSensor_0
# this script dumps the sensor values of a given device/sensor pair

if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    echo "Need the device id"
    echo "e.g. get_sensor_values.sh 62c7c657127dbd00011540a6 temperatureSensor_0"
    exit
fi

echo "--> Get sensor's values from device $1 sensor $2"

curl -X GET "http://localhost/devices/$1/sensors/$2/values" -H  "accept: application/json" > $1.$2.data.json

echo "--> Wrote data to $1.$2.data.json"