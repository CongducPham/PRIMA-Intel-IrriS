#!/bin/bash

logger -t intel-irris-auto-config "create-default-capacitive"

cd /home/pi/scripts
./delete_sensor.sh
./create_full_capacitive_device.sh