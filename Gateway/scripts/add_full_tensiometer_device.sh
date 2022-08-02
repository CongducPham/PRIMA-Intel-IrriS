#!/bin/bash

# this script adds a watermark sensor as secondary device, i.e. SOIL-AREA-2 and devAddr=26011DB2
# could be replaced by create_full_tensiometer_device_with_dev_addr.sh 2 B2

echo "--> Get token"
TOK=`curl -X POST "http://localhost/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

echo "--> Adding an additional device to the existing device"
echo "--> Create new device"

DEVICE=`curl -X POST "http://localhost/devices" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"actuators\":[],\"name\":\"SOIL-AREA-2\",\"sensors\":[{\"id\":\"temperatureSensor_0\",\"kind\":\"\",\"meta\":{\"createdBy\":\"wazigate-lora\",\"kind\":\"centibars from WM200\",\"model\":\"WM200\",\"type\":\"tensiometer\",\"sensor_dry_max\":124,\"sensor_wet_max\":0,\"sensor_n_interval\":6,\"value_index\":0},\"name\":\"Soil Humidity Sensor\",\"quantity\":\"\",\"time\":\"2022-04-06T14:39:45.205Z\",\"unit\":\"\",\"value\":10},{\"id\":\"temperatureSensor_1\",\"kind\":\"\",\"meta\":{\"createdBy\":\"wazigate-lora\",\"kind\":\"Scaled value from WM200 real=x10\",\"model\":\"WM200\",\"type\":\"tensiometer\",\"sensor_dry_max\":18000,\"sensor_wet_max\":0,\"sensor_n_interval\":6,\"value_index\":0},\"name\":\"Soil Humidity Sensor\",\"quantity\":\"\",\"time\":\"2022-04-06T14:39:45.205Z\",\"unit\":\"\",\"value\":550}]}" | tr -d '\"'`

echo $DEVICE > /home/pi/scripts/LAST_CREATED_DEVICE.txt
echo "device $DEVICE"
echo "		name: SOIL-AREA-2"
echo "		with Soil Humidity Sensor displaying centibars from WM200"
echo "		and initialized with 10 value"

echo "--> Make it LoRaWAN"
echo "		device id: 26011DB2"
curl -X POST "http://localhost/devices/${DEVICE}/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d  "{\"codec\":\"application/x-xlpp\",\"lorawan\":{\"appSKey\":\"23158D3BBC31E6AF670D195B5AED5525\",\"devAddr\":\"26011DB2\",\"devEUI\":\"AA555A0026011DB2\",\"nwkSEncKey\":\"23158D3BBC31E6AF670D195B5AED5525\",\"profile\":\"WaziDev\"}}"

echo "--> Add value 0"
curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_0/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":0, \"time\":\"2022-04-06T14:39:45.205Z\"}"

echo "--> Add value 0"
curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_1/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":0, \"time\":\"2022-04-06T14:39:45.205Z\"}"

#we adopt the following rule: 0:very dry; 1:dry; 2:dry-wet 3-wet-dry; 4-wet; 5-very wet
echo "--> Change humidity index to max value - 5:very wet"
curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_0/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"

curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_1/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"