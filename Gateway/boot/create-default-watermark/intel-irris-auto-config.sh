#!/bin/bash

logger -t intel-irris-auto-config "create-default-tensiometer"

cd /home/pi/scripts
./delete_sensor.sh
./create_full_tensiometer_device.sh