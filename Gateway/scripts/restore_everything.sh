#!/bin/bash

# This script restores all from the sensor-backup folder.
# It also adds a default IIWA setup for all restored sensor.
# Example:
# > ./scripts/restore_everything.sh

cd /home/pi/sensor-backup

/home/pi/scripts/delete_all_devices.sh

capas=$(ls | grep split | grep capa | awk -F'[_.]' '{print $1}' | uniq)
# echo $capas
for k in $capas
do
	# echo $k
	devname=$(cat sensor-backup.log | grep $k | grep backup | awk -F'[ ]' '{print $6}' | awk -F'[-]' '{print $3}')
	devaddr=$(cat sensor-backup.log | grep $k | grep backup | awk -F'[ ]' '{print $8}')

	/home/pi/scripts/restore_capacitive_device_sensor_values.sh $devname $devaddr $k $k
	/home/pi/scripts/iiwa_rest.sh add $k SOIL-AREA-$devname 1_capacitive temperatureSensor_0

done

tensios=$(ls | grep split | grep tensio | awk -F'[_.]' '{print $1}' | uniq)
for k in $tensios
do
	# echo $k
	devname=$(cat sensor-backup.log | grep $k | grep backup | awk -F'[ ]' '{print $6}' | awk -F'[-]' '{print $3}')
	devaddr=$(cat sensor-backup.log | grep $k | grep backup | awk -F'[ ]' '{print $8}')

	is2WT=$(ls | grep $k | grep "ensor\_2" | wc -l)
	if [ $is2WT -eq 0 ]
	then
		# echo "single"
		/home/pi/scripts/restore_tensiometer_device_sensor_values.sh $devname $devaddr $k $k
		/home/pi/scripts/iiwa_rest.sh add $k SOIL-AREA-$devname 1_watermark temperatureSensor_0
	else
		# echo "2WT"
		/home/pi/scripts/restore_2-tensiometer_device_sensor_values.sh $devname $devaddr $k $k
		/home/pi/scripts/iiwa_rest.sh add $k SOIL-AREA-$devname 2_watermark temperatureSensor_0
		/home/pi/scripts/iiwa_rest.sh add $k SOIL-AREA-$devname 2_watermark temperatureSensor_2
	fi
done

# restore IIWA configs
SENSOR_CONFIGS=`cat backup_iiwa.json | jq`
NSENSORS=`echo $SENSOR_CONFIGS | jq '.sensors | length'`

while [ $NSENSORS -gt 0 ]
do
    (( NSENSORS-- ))
    DEVICE=`echo $SENSOR_CONFIGS | jq ".sensors[${NSENSORS}].device_id"  | tr -d '\"'`
    SENSOR=`echo $SENSOR_CONFIGS | jq ".sensors[${NSENSORS}].sensor_id"  | tr -d '\"'`
    DEVCONF=`echo $SENSOR_CONFIGS | jq ".sensors[${NSENSORS}]"`

    sensor_type=`echo $DEVCONF | jq ".value.sensor_type" | tr -d '\"'`
    sensor_age=`echo $DEVCONF | jq ".value.sensor_age" | tr -d '\"'`
    sensor_max_value=`echo $DEVCONF | jq ".value.sensor_max_value" | tr -d '\"'`
    sensor_min_value=`echo $DEVCONF | jq ".value.sensor_min_value" | tr -d '\"'`
    soil_type=`echo $DEVCONF | jq ".value.soil_type" | tr -d '\"'`
    soil_irrigation_type=`echo $DEVCONF | jq ".value.soil_irrigation_type" | tr -d '\"'`
    soil_salinity=`echo $DEVCONF | jq ".value.soil_salinity" | tr -d '\"'`
    soil_bulk_density=`echo $DEVCONF | jq ".value.soil_bulk_density" | tr -d '\"'`
    soil_field_capacity=`echo $DEVCONF | jq ".value.soil_field_capacity" | tr -d '\"'`
    soil_temperature_value=`echo $DEVCONF | jq ".soil_temperature_source.soil_temperature_value" | tr -d '\"'`
    soil_temperature_device_id=`echo $DEVCONF | jq ".soil_temperature_source.soil_temperature_device_id" | tr -d '\"'`
    soil_temperature_sensor_id=`echo $DEVCONF | jq ".soil_temperature_source.soil_temperature_sensor_id" | tr -d '\"'`
    plant_category=`echo $DEVCONF | jq ".value.plant_category" | tr -d '\"'`
    plant_type=`echo $DEVCONF | jq ".value.plant_type" | tr -d '\"'`
    plant_variety=`echo $DEVCONF | jq ".value.plant_variety" | tr -d '\"'`
    plant_planting_date=`echo $DEVCONF | jq ".value.plant_planting_date" | tr -d '\"'`
    weather_region=`echo $DEVCONF | jq ".value.weather_region" | tr -d '\"'`
    weather_weekly_evaporation=`echo $DEVCONF | jq ".value.weather_weekly_evaporation" | tr -d '\"'`
    weather_weekly_pluviometry=`echo $DEVCONF | jq ".value.weather_weekly_pluviometry" | tr -d '\"'`


    NEWCONF="
    {
        \"sensor_type\":\"$sensor_type\",
        \"sensor_age\":\"$sensor_age\",
        \"sensor_max_value\":\"$sensor_max_value\",
        \"sensor_min_value\":\"$sensor_min_value\",
        \"soil_type\":\"$soil_type\",
        \"soil_irrigation_type\":\"$soil_irrigation_type\",
        \"soil_salinity\":\"$soil_salinity\",
        \"soil_bulk_density\":\"$soil_bulk_density\",
        \"soil_field_capacity\":\"$soil_field_capacity\",
        \"soil_temperature_value\":\"$soil_temperature_value\",
        \"soil_temperature_device_id\":\"$soil_temperature_device_id\",
        \"soil_temperature_sensor_id\":\"$soil_temperature_sensor_id\",
        \"plant_category\":\"$plant_category\",
        \"plant_type\":\"$plant_type\",
        \"plant_variety\":\"$plant_variety\",
        \"plant_planting_date\":\"$plant_planting_date\",
        \"weather_region\":\"$weather_region\",
        \"weather_weekly_evaporation\":\"$weather_weekly_evaporation\",
        \"weather_weekly_pluviometry\":\"$weather_weekly_pluviometry\"
    }
    "

    update_data=`echo $NEWCONF | jq`
    # echo "$update_data"
	/home/pi/scripts/iiwa_rest.sh update $DEVICE $SENSOR "$update_data"

done