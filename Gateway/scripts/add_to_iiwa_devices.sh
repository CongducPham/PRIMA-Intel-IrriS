#!/bin/bash

# Ex: add_to_iiwa_devices.sh 63c04492ca473d6e95aaa220 1 capacitive
# Ex: add_to_iiwa_devices.sh 63c04493ca473d6e95aaa224 2 tensiometer

if [ $3 == 'tensiometer' ]
then
	STYPE="watermark"
else
	STYPE=$3
fi

tmpfile=$(mktemp)

jq ". + [{ \
    \"device_id\": \"${1}\", \
    \"device_name\": \"SOIL-AREA-${2}\", \
    \"sensors_structure\": \"1_${STYPE}\" \
    }]" intel_irris_devices.json > "$tmpfile" && mv -- "$tmpfile" intel_irris_devices.json

