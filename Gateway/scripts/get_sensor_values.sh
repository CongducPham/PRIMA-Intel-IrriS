#!/bin/bash

# Ex: get_sensor_values
# this script dumps the sensor values

if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    echo "Need the device id"
    echo "e.g. get_sensor_values 62c7c657127dbd00011540a6 temperatureSensor_0"
    exit
fi

echo "--> Get token"
TOK=`curl -X POST "http://localhost/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

DATE=`date +"%Y-%m-%dT%H:%M:%S.%3NZ"`

echo "--> date is $DATE" 
echo "--> Get sensor's values from device $1 sensor $2"

curl -X GET "http://localhost/devices/$1/sensors/$2/values" -H  "accept: application/json" > $1.$2.data.json

echo "--> Wrote data to $1.$2.data.json"