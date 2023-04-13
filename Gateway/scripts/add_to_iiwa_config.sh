#!/bin/bash

# Ex: add_to_iiwa_config.sh 63c04492ca473d6e95aaa220 capacitive
# Ex: add_to_iiwa_config.sh 63c04493ca473d6e95aaa224 tensiometer
# Ex: add_to_iiwa_config.sh 63c04493ca473d6e95aaa224 2tensiometers


tmpfile=$(mktemp)

if [ $2 == 'capacitive' ]
then
jq ".sensors += \
    [{\
      \"value\": {\
        \"sensor_type\": \"${2}\",\
        \"sensor_age\": \"0\",\
        \"sensor_max\": \"800\",\
        \"sensor_min\": \"0\",\
        \"soil_type\": \"silty\",\
        \"soil_irrigation_type\": \"undefined\",\
        \"soil_salinity\": \"undefined\",\
        \"soil_bulk_density\": \"undefined\",\
        \"plant_crop\": \"undefined\",\
        \"plant_sub_type\": \"undefined\",\
        \"plant_planting_date\": \"undefined\",\
        \"weather_region\": \"semi-arid\",\
        \"last_value\": 0\
      },\
      \"soil_temperature_source\": {\
        \"soil_temperature_device_id\": \"undefined\",\
        \"soil_temperature_sensor_id\": \"undefined\",\
        \"soil_temperature_value\": \"undefined\"\
      },\
      \"device_id\": \"${1}\",\
      \"sensor_id\": \"temperatureSensor_0\"\
    }]" intel_irris_sensors_configurations.json  > "$tmpfile" && mv -- "$tmpfile" intel_irris_sensors_configurations.json  
fi

if [ $2 == 'tensiometer' ]
then  
jq ".sensors += \
    [{\
      \"value\": {\
        \"sensor_type\": \"${2}_cbar\",\
        \"sensor_age\": \"0\",\
        \"sensor_max\": \"124\",\
        \"sensor_min\": \"0\",\
        \"soil_type\": \"silty\",
        \"soil_irrigation_type\": \"undefined\",\
        \"soil_salinity\": \"undefined\",\
        \"soil_bulk_density\": \"undefined\",\
        \"plant_crop\": \"undefined\",\
        \"plant_sub_type\": \"undefined\",\
        \"plant_planting_date\": \"undefined\",\
        \"weather_region\": \"semi-arid\",\
        \"last_value\": 0\
      },\
      \"soil_temperature_source\": {\
        \"soil_temperature_device_id\": \"${1}\",\
        \"soil_temperature_sensor_id\": \"temperatureSensor_5\",\
        \"soil_temperature_value\": \"undefined\"\
      },\
      \"device_id\": \"${1}\",\
      \"sensor_id\": \"temperatureSensor_0\"\
    }]" intel_irris_sensors_configurations.json  > "$tmpfile" && mv -- "$tmpfile" intel_irris_sensors_configurations.json  
fi

if [ $2 == '2tensiometers' ]
then  
jq ".sensors += \
    [{\
      \"value\": {\
        \"sensor_type\": \"tensiometer_cbar\",\
        \"sensor_age\": \"0\",\
        \"sensor_max\": \"124\",\
        \"sensor_min\": \"0\",\
        \"soil_type\": \"silty\",
        \"soil_irrigation_type\": \"undefined\",\
        \"soil_salinity\": \"undefined\",\
        \"soil_bulk_density\": \"undefined\",\
        \"plant_crop\": \"undefined\",\
        \"plant_sub_type\": \"undefined\",\
        \"plant_planting_date\": \"undefined\",\
        \"weather_region\": \"semi-arid\",\
        \"last_value\": 0\
      },\
      \"soil_temperature_source\": {\
        \"soil_temperature_device_id\": \"${1}\",\
        \"soil_temperature_sensor_id\": \"temperatureSensor_5\",\
        \"soil_temperature_value\": \"undefined\"\
      },\
      \"device_id\": \"${1}\",\
      \"sensor_id\": \"temperatureSensor_0\"\
    }]" intel_irris_sensors_configurations.json  > "$tmpfile" && mv -- "$tmpfile" intel_irris_sensors_configurations.json  
jq ".sensors += \
    [{\
      \"value\": {\
        \"sensor_type\": \"tensiometer_cbar\",\
        \"sensor_age\": \"0\",\
        \"sensor_max\": \"124\",\
        \"sensor_min\": \"0\",\
        \"soil_type\": \"silty\",
        \"soil_irrigation_type\": \"undefined\",\
        \"soil_salinity\": \"undefined\",\
        \"soil_bulk_density\": \"undefined\",\
        \"plant_crop\": \"undefined\",\
        \"plant_sub_type\": \"undefined\",\
        \"plant_planting_date\": \"undefined\",\
        \"weather_region\": \"semi-arid\",\
        \"last_value\": 0\
      },\
      \"soil_temperature_source\": {\
        \"soil_temperature_device_id\": \"${1}\",\
        \"soil_temperature_sensor_id\": \"temperatureSensor_5\",\
        \"soil_temperature_value\": \"undefined\"\
      },\
      \"device_id\": \"${1}\",\
      \"sensor_id\": \"temperatureSensor_2\"\
    }]" intel_irris_sensors_configurations.json  > "$tmpfile" && mv -- "$tmpfile" intel_irris_sensors_configurations.json      
fi