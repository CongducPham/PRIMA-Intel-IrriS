#!/bin/bash

if [ $# -eq 0 ]
	then
		echo "No arguments supplied"
		echo "Taking default device id"
		DEVICE="62286d72f06c4c0001eba943"
	else
		DEVICE=$1
fi

echo "Get token"
TOK=`curl -X POST "http://localhost/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

echo "Delete device ${DEVICE}"
curl -X DELETE "http://localhost/devices/${DEVICE}" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json"   

echo "Checking delete operation"
curl -X GET "http://localhost/devices" -H  "accept: application/json"