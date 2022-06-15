#!/bin/bash

if [ $# -eq 0 ]
	then
		echo "No arguments supplied"
		echo "Taking default device id"
		device_id="62286d72f06c4c0001eba943"
	else
		device_id=$1
fi

echo "Get token"
TOK=`curl -X POST "http://localhost/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

echo "Delete device $1"
curl -X DELETE "http://localhost/devices/$device_id" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json"

echo "Checking delete operation"
curl -X GET "http://localhost/devices" -H  "accept: application/json"

