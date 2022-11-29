#!/bin/bash

# this script configures IIWA's configuration files after creation of the default device/sensor
cd /home/pi/scripts

echo "--> IIWA configuration"

echo "--> get default device id"
DEV_ID=`./show_device_by_name.sh SOIL-AREA-1 id | tr -d '\"'`

echo "--> get default sensor type"
SEN_TYPE=`./show_device_by_name.sh SOIL-AREA-1 sensors[0].meta.model | tr -d '\"'`


