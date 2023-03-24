#!/bin/bash

#inspired from delete_all_device.sh and get_sensor_values.sh

# empty previous file
echo "" > extracted.json
echo "" > extracted_meta.json

#echo "Get token"
TOK=`curl -X POST "http://localhost/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

#echo "Get number of devices"
NDEVICE=`curl -X GET "http://localhost/devices" -H  "accept: application/json" | jq '. | length'`

(( NDEVICE-- ))

#echo $NDEVICE

#we leave device[0] which is usually the initial gateway declaration
while [ $NDEVICE -gt 0 ]
do
  #get variables for json bribes identifying 
  DEVICE=`curl -X GET "http://localhost/devices" -H  "accept: application/json" | jq ".[$NDEVICE].id" | tr -d '\"'`
  DEVNAME=`curl -X GET "http://localhost/devices" -H  "accept: application/json" | jq ".[$NDEVICE].name" | tr -d '\"'`
  DEVADDR=`curl -X GET "http://localhost/devices" -H  "accept: application/json" | jq ".[$NDEVICE].meta.lorawan.devAddr" | tr -d '\"'`
  DEVSENSORS=`curl -X GET "http://localhost/devices" -H  "accept: application/json" | jq ".[$NDEVICE].sensors"`
  echo "${DEVNAME}:${DEVADDR}:${DEVICE}" >> extracted_meta.json
  echo "${DEVSENSORS}" >> extracted_meta.json
  
  sizeDEVICE=${#DEVICE}
  
  #we do not want to go through a gateway as it is also considered as a device
  if [ $sizeDEVICE -gt 12 ]
  then
    echo "${DEVNAME}:${DEVADDR}:${DEVICE}" >> extracted.json
    curl -X GET "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_0/values" -H  "accept: application/json" >> extracted.json
    curl -X GET "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_1/values" -H  "accept: application/json" >> extracted.json
    curl -X GET "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_5/values" -H  "accept: application/json" >> extracted.json
    curl -X GET "http://localhost/devices/${DEVICE}/sensors/analogInput_6/values" -H  "accept: application/json" >> extracted.json
    echo "" >> extracted.json
  else
    echo "Probably a gateway, skipping"  
  fi
      
  (( NDEVICE-- ))
  echo $NDEVICE  
done

echo "--> Wrote data to extracted.json"
