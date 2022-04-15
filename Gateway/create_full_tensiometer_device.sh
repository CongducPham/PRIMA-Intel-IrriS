#!/bin/bash

echo "--> Get token"
TOK=`curl -X POST "http://localhost/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

echo "--> Create device 62286d72f06c4c0001eba943"
curl -X POST "http://localhost/devices" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"actuators\":[],\"id\":\"62286d72f06c4c0001eba943\",\"name\":\"INTEL-IRRIS Soil Sensor\",\"sensors\":[{\"id\":\"temperatureSensor_0\",\"kind\":\"\",\"meta\":{\"createdBy\":\"wazigate-lora\",\"kind\":\"Raw value from WM200\",\"model\":\"WM200\",\"type\":\"tensiometer\",\"sensor_dry_max\":550,\"sensor_wet_max\":0,\"sensor_n_interval\":6,\"value_index\":0},\"name\":\"Soil Humidity Sensor\",\"quantity\":\"\",\"time\":\"2022-04-06T14:39:45.205Z\",\"unit\":\"\",\"value\":550}]}"
echo "		name: INTEL-IRRIS Soil Sensor"
echo "		with Soil Humidity Sensor displaying Raw value from WM200"
echo "		and initialized with 550 value"

echo "--> Make it LoRaWAN"
echo "	device id: 26011DBB"
curl -X POST "http://localhost/devices/62286d72f06c4c0001eba943/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d  "{\"codec\":\"application/x-xlpp\",\"lorawan\":{\"appSKey\":\"23158D3BBC31E6AF670D195B5AED5525\",\"devAddr\":\"26011DBB\",\"devEUI\":\"AA555A0026011DBB\",\"nwkSEncKey\":\"23158D3BBC31E6AF670D195B5AED5525\",\"profile\":\"WaziDev\"}}"

echo "--> Add value 0"
curl -X POST "http://localhost/devices/62286d72f06c4c0001eba943/sensors/temperatureSensor_0/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":0}"

#we adopt the following rule: 0:very dry; 1:dry; 2:dry-wet 3-wet-dry; 4-wet; 5-very wet
echo "--> Change humidity index to max value - 5:very wet"
curl -X POST "http://localhost/devices/62286d72f06c4c0001eba943/sensors/temperatureSensor_0/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"