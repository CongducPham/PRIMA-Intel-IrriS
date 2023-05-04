#!/bin/bash

# Remote equivalent of create-custom-multiple-iiwa.
# Without keeping the conf in a devices.txt file.
# Instead of modifying all the scripts to make them remote, direct curl invocation is involved here

if [ $# -eq 0 ]
then
  echo "No arguments supplied"
  echo "Need a destination URL."
  echo "Need a sequence of device type, name, and address."
  echo "e.g. remote-custom-multiple-iiwa.sh wazigate.local C 1 AE WT 8 B4"
  echo "will create 1 capacitive SOIL-AREA-1 with address 26011DAE"
  echo "and 1 watermark with temperature sensor SOIL-AREA-8 with address 26011DB4"
  echo "for a double WM + temperature : 2WT"
  exit
fi


TOK=`curl -X POST "http://$1/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

NDEVICE=`curl -X GET "http://$1/devices" -H  "accept: application/json" -H "Authorization: Bearer $TOK" | jq '. | length'`

(( NDEVICE-- ))

#we leave device[0] which is usually the initial gateway declaration
while [ $NDEVICE -gt 0 ]
do
  DEVICE=`curl -X GET "http://$1/devices" -H  "accept: application/json" -H "Authorization: Bearer $TOK" | jq ".[$NDEVICE].id" | tr -d '\"'`
  echo "Device ${DEVICE}"
  
  sizeDEVICE=${#DEVICE}
  
  #we do not want to delete a gateway as it is also considered as a device
  if [ $sizeDEVICE -gt 16 ]
  then
    echo "Delete device ${DEVICE}"
    curl -X DELETE "http://$1:5000/devices/${DEVICE}" -H  "accept: application/json"
    curl -X DELETE "http://$1/devices/${DEVICE}" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json"   
  else
    echo "Probably a gateway, skipping"  
  fi
      
  (( NDEVICE-- ))
  echo $NDEVICE  
done

echo "Checking delete operation"
curl -X GET "http://$1/devices" -H  "accept: application/json" -H "Authorization: Bearer $TOK"


# iterate over devices
arg_id=0
for var in "$@"
do
  if [ $arg_id -eq 0 ]
    then
      echo $var
  elif [ $((arg_id%3)) -eq 1 ]
    then
      DEVTYPE=$var
  elif [ $((arg_id%3)) -eq 2 ]
    then
      DEVID=$var
  elif [ $((arg_id%3)) -eq 0 ]
    then
      DEVADDR=$var
  fi
  arg_id=$((arg_id+1))
  if [ $((arg_id%3)) -eq 1 ]
    then
      if [ $arg_id != 1 ]
        then

          if [ "$DEVTYPE" = "C" ] || [ "$DEVTYPE" = "CT" ] || [ "$DEVTYPE" = "WT" ] || [ "$DEVTYPE" = "2WT" ] ; then
            DATE=`date +"%Y-%m-%dT06:00:00.001Z"`
            if [ "$DEVTYPE" = "C" ]|| [ "$DEVTYPE" = "CT" ]; then
              #create capacitive SOIL-AREA-1 (i) and device with address 26011DAA, Ab, Ac,...
              
              DEVICE=`curl -X POST "http://$1/devices" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
                \"actuators\":[],
                \"name\":\"SOIL-AREA-${DEVID}\",
                \"sensors\":[
                {
                  \"id\":\"temperatureSensor_0\",
                  \"kind\":\"\",
                  \"meta\":
                  {
                    \"xlppChan\":0,
                    \"createdBy\":\"wazigate-lora\",
                    \"kind\":\"Raw value from SEN0308\",
                    \"model\":\"SEN0308\",
                    \"type\":\"capacitive\",
                    \"sensor_dry_max\":800,
                    \"sensor_wet_max\":0,
                    \"sensor_n_interval\":6,
                    \"value_index\":0
                  },
                  \"name\":\"Soil Humidity Sensor\",
                  \"quantity\":\"\",
                  \"time\":\"$DATE\",
                  \"unit\":\"\",
                  \"value\":800
                }]}" | tr -d '\"'`
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_0/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":-1, \"time\":\"$DATE\"}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_0/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"

              curl -X POST "http://$1:5000/devices/${DEVICE}" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
                \"device_id\":\"${DEVICE}\",\"device_name\":\"SOIL-AREA-${DEVID}\",\"sensors_structure\":\"1_capacitive\"}" | tr -d '\"'

              curl -X POST "http://$1:5000/devices/${DEVICE}/sensors/temperatureSensor_0" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "
              {
                  \"sensor_type\": \"capacitive\", 
                  \"sensor_age\": \"0\", 
                  \"sensor_max_value\": \"800\", 
                  \"sensor_min_value\": \"0\", 
                  \"soil_type\": \"silty\", 
                  \"soil_irrigation_type\": \"undefined\", 
                  \"soil_salinity\": \"undefined\", 
                  \"soil_bulk_density\": \"undefined\",
                  \"soil_field_capacity\":\"undefined\", 
                  \"soil_temperature_value\": \"undefined\", 
                  \"soil_temperature_device_id\": \"undefined\", 
                  \"soil_temperature_sensor_id\": \"undefined\", 
                  \"plant_category\":\"undefined\",
                  \"plant_type\":\"undefined\",
                  \"plant_variety\":\"undefined\",
                  \"plant_planting_date\": \"undefined\", 
                  \"weather_region\": \"semi-arid\", 
                  \"weather_weekly_evaporation\":\"undefined\",
                  \"weather_weekly_pluviometry\":\"undefined\"
              }
              " | tr -d '\"'            

            elif [ "$DEVTYPE" = "WT" ]; then
              DEVICE=`curl -X POST "http://$1/devices" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
                \"actuators\":[],
                \"name\":\"SOIL-AREA-${DEVID}\",
                \"sensors\":[
                {
                  \"id\":\"temperatureSensor_0\",
                  \"kind\":\"\",
                  \"meta\":
                  {
                    \"xlppChan\":0,
                    \"createdBy\":\"wazigate-lora\",
                    \"kind\":\"centibars from WM200\",
                    \"model\":\"WM200\",
                    \"type\":\"tensiometer\",
                    \"sensor_dry_max\":124,
                    \"sensor_wet_max\":0,
                    \"sensor_n_interval\":6,
                    \"value_index\":0
                  },
                  \"name\":\"Soil Humidity Sensor\",
                  \"quantity\":\"\",
                  \"time\":\"$DATE\",
                  \"unit\":\"\",
                  \"value\":10
                },{
                  \"id\":\"temperatureSensor_1\",
                  \"kind\":\"\",
                  \"meta\":
                  {
                    \"xlppChan\":1,
                    \"createdBy\":\"wazigate-lora\",
                    \"kind\":\"scaled value from WM200 real=x10\",
                    \"model\":\"WM200\",
                    \"type\":\"tensiometer\",
                    \"sensor_dry_max\":18000,
                    \"sensor_wet_max\":0,
                    \"sensor_n_interval\":6,
                    \"value_index\":0
                  },
                  \"name\":\"Soil Humidity Sensor\",
                  \"quantity\":\"\",
                  \"time\":\"$DATE\",
                  \"unit\":\"\",
                  \"value\":550
                }]}" | tr -d '\"'`

              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_0/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":255, \"time\":\"$DATE\"}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_1/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":"3276", \"time\":\"$DATE\"}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_0/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_1/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"

              curl -X POST "http://$1:5000/devices/${DEVICE}" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
                \"device_id\":\"${DEVICE}\",\"device_name\":\"SOIL-AREA-${DEVID}\",\"sensors_structure\":\"1_watermark\"}" | tr -d '\"'

              curl -X POST "http://$1:5000/devices/${DEVICE}/sensors/temperatureSensor_0" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "
              {
                  \"sensor_type\": \"tensiometer_cbar\", 
                  \"sensor_age\": \"0\", 
                  \"sensor_max_value\": \"124\", 
                  \"sensor_min_value\": \"0\", 
                  \"soil_type\": \"silty\", 
                  \"soil_irrigation_type\": \"undefined\", 
                  \"soil_salinity\": \"undefined\", 
                  \"soil_bulk_density\": \"undefined\",
                  \"soil_field_capacity\":\"undefined\", 
                  \"soil_temperature_value\": \"undefined\", 
                  \"soil_temperature_device_id\": \"${DEVICE}\", 
                  \"soil_temperature_sensor_id\": \"temperatureSensor_5\", 
                  \"plant_category\":\"undefined\",
                  \"plant_type\":\"undefined\",
                  \"plant_variety\":\"undefined\",
                  \"plant_planting_date\": \"undefined\", 
                  \"weather_region\": \"semi-arid\", 
                  \"weather_weekly_evaporation\":\"undefined\",
                  \"weather_weekly_pluviometry\":\"undefined\"
              }
              " | tr -d '\"'



            elif [ "$DEVTYPE" = "2WT" ]; then
              DEVICE=`curl -X POST "http://$1/devices" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
                \"actuators\":[],
                \"name\":\"SOIL-AREA-${DEVID}\",
                \"sensors\":[
                {
                  \"id\":\"temperatureSensor_0\",
                  \"kind\":\"\",
                  \"meta\":
                  {
                    \"xlppChan\":0,
                    \"createdBy\":\"wazigate-lora\",
                    \"kind\":\"centibars from WM200\",
                    \"model\":\"WM200\",
                    \"type\":\"tensiometer\",
                    \"sensor_dry_max\":124,
                    \"sensor_wet_max\":0,
                    \"sensor_n_interval\":6,
                    \"value_index\":0
                  },
                  \"name\":\"Watermark 1\",
                  \"quantity\":\"\",
                  \"time\":\"$DATE\",
                  \"unit\":\"\",
                  \"value\":10
                },{
                  \"id\":\"temperatureSensor_1\",
                  \"kind\":\"\",
                  \"meta\":
                  {
                    \"xlppChan\":1,
                    \"createdBy\":\"wazigate-lora\",
                    \"kind\":\"scaled value from WM200 real=x10\",
                    \"model\":\"WM200\",
                    \"type\":\"tensiometer\",
                    \"sensor_dry_max\":18000,
                    \"sensor_wet_max\":0,
                    \"sensor_n_interval\":6,
                    \"value_index\":0
                  },
                  \"name\":\"Watermark 1\",
                  \"quantity\":\"\",
                  \"time\":\"$DATE\",
                  \"unit\":\"\",
                  \"value\":550
                },{
                  \"id\":\"temperatureSensor_2\",
                  \"kind\":\"\",
                  \"meta\":
                  {
                    \"xlppChan\":2,
                    \"createdBy\":\"wazigate-lora\",
                    \"kind\":\"centibars from WM200\",
                    \"model\":\"WM200\",
                    \"type\":\"tensiometer\",
                    \"sensor_dry_max\":124,
                    \"sensor_wet_max\":0,
                    \"sensor_n_interval\":6,
                    \"value_index\":0
                  },
                  \"name\":\"Watermark 2\",
                  \"quantity\":\"\",
                  \"time\":\"$DATE\",
                  \"unit\":\"\",
                  \"value\":10
                },{
                  \"id\":\"temperatureSensor_3\",
                  \"kind\":\"\",
                  \"meta\":
                  {
                    \"xlppChan\":3,
                    \"createdBy\":\"wazigate-lora\",
                    \"kind\":\"scaled value from WM200 real=x10\",
                    \"model\":\"WM200\",
                    \"type\":\"tensiometer\",
                    \"sensor_dry_max\":18000,
                    \"sensor_wet_max\":0,
                    \"sensor_n_interval\":6,
                    \"value_index\":0
                  },
                  \"name\":\"Watermark 2\",
                  \"quantity\":\"\",
                  \"time\":\"$DATE\",
                  \"unit\":\"\",
                  \"value\":550
                }]}" | tr -d '\"'`

              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_0/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":255, \"time\":\"$DATE\"}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_1/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":3276, \"time\":\"$DATE\"}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_2/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":255, \"time\":\"$DATE\"}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_3/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":3276, \"time\":\"$DATE\"}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_0/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_1/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_2/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"
              curl -X POST "http://$1/devices/${DEVICE}/sensors/temperatureSensor_3/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value_index\":5}"

              curl -X POST "http://$1:5000/devices/${DEVICE}" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
                \"device_id\":\"${DEVICE}\",\"device_name\":\"SOIL-AREA-${DEVID}\",\"sensors_structure\":\"2_watermark\"}" | tr -d '\"'

              curl -X POST "http://$1:5000/devices/${DEVICE}/sensors/temperatureSensor_0" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "
              {
                  \"sensor_type\": \"tensiometer_cbar\", 
                  \"sensor_age\": \"0\", 
                  \"sensor_max_value\": \"124\", 
                  \"sensor_min_value\": \"0\", 
                  \"soil_type\": \"silty\", 
                  \"soil_irrigation_type\": \"undefined\", 
                  \"soil_salinity\": \"undefined\", 
                  \"soil_bulk_density\": \"undefined\",
                  \"soil_field_capacity\":\"undefined\", 
                  \"soil_temperature_value\": \"undefined\", 
                  \"soil_temperature_device_id\": \"${DEVICE}\", 
                  \"soil_temperature_sensor_id\": \"temperatureSensor_5\", 
                  \"plant_category\":\"undefined\",
                  \"plant_type\":\"undefined\",
                  \"plant_variety\":\"undefined\",
                  \"plant_planting_date\": \"undefined\", 
                  \"weather_region\": \"semi-arid\", 
                  \"weather_weekly_evaporation\":\"undefined\",
                  \"weather_weekly_pluviometry\":\"undefined\"
              }
              " | tr -d '\"'


              curl -X POST "http://$1:5000/devices/${DEVICE}" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
                \"device_id\":\"${DEVICE}\",\"device_name\":\"SOIL-AREA-${DEVID}\",\"sensors_structure\":\"2_watermark\"}" | tr -d '\"'

              curl -X POST "http://$1:5000/devices/${DEVICE}/sensors/temperatureSensor_2" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "
              {
                  \"sensor_type\": \"tensiometer_cbar\", 
                  \"sensor_age\": \"0\", 
                  \"sensor_max_value\": \"124\", 
                  \"sensor_min_value\": \"0\", 
                  \"soil_type\": \"silty\", 
                  \"soil_irrigation_type\": \"undefined\", 
                  \"soil_salinity\": \"undefined\", 
                  \"soil_bulk_density\": \"undefined\",
                  \"soil_field_capacity\":\"undefined\", 
                  \"soil_temperature_value\": \"undefined\", 
                  \"soil_temperature_device_id\": \"${DEVICE}\", 
                  \"soil_temperature_sensor_id\": \"temperatureSensor_5\", 
                  \"plant_category\":\"undefined\",
                  \"plant_type\":\"undefined\",
                  \"plant_variety\":\"undefined\",
                  \"plant_planting_date\": \"undefined\", 
                  \"weather_region\": \"semi-arid\", 
                  \"weather_weekly_evaporation\":\"undefined\",
                  \"weather_weekly_pluviometry\":\"undefined\"
              }
              " | tr -d '\"'

            fi

            curl -X POST "http://$1/devices/${DEVICE}/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d  "{\"codec\":\"application/x-xlpp\",\"lorawan\":{\"appSKey\":\"23158D3BBC31E6AF670D195B5AED5525\",\"devAddr\":\"26011D${DEVADDR}\",\"devEUI\":\"AA555A0026011D${DEVADDR}\",\"nwkSEncKey\":\"23158D3BBC31E6AF670D195B5AED5525\",\"profile\":\"WaziDev\"}}"
            curl -X POST "http://$1/devices/${DEVICE}/sensors" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
              \"id\":\"analogInput_6\",
              \"kind\":\"\",
              \"meta\":
              {
                \"xlppChan\":6,
                \"createdBy\":\"wazigate-lora\",
                \"kind\":\"volt, low battery when lower than 2.85V\"
              },
              \"name\":\"Battery voltage\",
              \"quantity\":\"\",
              \"time\":\"$DATE\",
              \"unit\":\"\",
              \"value\":-1
            }"

            if [ "$DEVTYPE" = "CT" ] || [ "$DEVTYPE" = "WT" ] || [ "$DEVTYPE" = "2WT" ]; then
              curl -X POST "http://$1/devices/${DEVICE}/sensors" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
                \"id\":\"temperatureSensor_5\",
                \"kind\":\"\",
                \"meta\":
                {
                  \"xlppChan\":5,
                  \"createdBy\":\"wazigate-lora\",
                  \"kind\":\"degree Celsius\",
                  \"model\":\"DS18B20\",
                  \"type\":\"temperature\",
                  \"value_index\":0
                },
                \"name\":\"Soil Temperature Sensor\",
                \"quantity\":\"\",
                \"time\":\"$DATE\",
                \"unit\":\"\",
                \"value\":-99
              }"
            fi

          else
              echo "Device type not recognized, skipping."
          fi
      fi
  fi
done


