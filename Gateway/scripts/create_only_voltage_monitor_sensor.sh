#!/bin/bash

# Ex: create_only_voltage_monitor_sensor.sh 62286d72f06c4c0001eba943
# this script creates a voltage monitor sensor attached to a given device
# the voltage monitor sensor is always temperatureSensor_6

if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    echo "Need the device id where the sensor will be added"
    echo "e.g. create_only_voltage_monitor_sensor.sh 62286d72f06c4c0001eba943"
    exit
fi

echo "--> Get token"
TOK=`curl -X POST "http://localhost/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

DATE=`date +"%Y-%m-%dT%H:%M:%S.%3NZ"`

echo "--> date is $DATE" 
echo "--> Use device ${1}"
echo "--> Create voltage monitor sensor"

curl -X POST "http://localhost/devices/${1}/sensors" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"id\":\"temperatureSensor_6\",\"kind\":\"\",\"meta\":{\"createdBy\":\"wazigate-lora\",\"kind\":\"volt, low battery when < 2.9\"},\"name\":\"Battery voltage\",\"quantity\":\"\",\"time\":\"2022-04-06T14:39:45.205Z\",\"unit\":\"\",\"value\":\"N/A\"}"

echo "device $1"
echo "		with voltage monitor displaying voltage"
echo "		and initialized with N/A value"

