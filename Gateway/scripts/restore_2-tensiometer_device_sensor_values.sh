#!/bin/bash

# Ex: restore_tensiometer_device_sensor_values.sh 1 B1 62c7c657127dbd00011540a6
# this script push data backup from a tensiometer w/soil temperature device to a new created tensiometer device

if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    echo "Need the original device id"
    echo "e.g. restore_2-tensiometer_device_sensor_values.sh 1 B1 62c7c657127dbd00011540a6"
    exit
fi

echo "--> Get token"
TOK=`curl -X POST "http://localhost/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

DATE=`date +"%Y-%m-%dT06:00:00.001Z"`

echo "--> Use date of $DATE"

if [ $# -eq 4 ]
then

# a specific device id has been given
echo "--> Create new tensiometer device with specific device id $4"

DEVICE=`curl -X POST "http://localhost/devices" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
  \"actuators\":[],
  \"id\":\"${4}\",
  \"name\":\"SOIL-AREA-${1}\",
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
    \"unit\":\"\"
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
    \"unit\":\"\"
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
    \"unit\":\"\"
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
    \"unit\":\"\"
  }]}" | tr -d '\"'`

else

echo "--> Create new tensiometer device"

DEVICE=`curl -X POST "http://localhost/devices" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
  \"actuators\":[],
  \"name\":\"SOIL-AREA-${1}\",
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
    \"unit\":\"\"
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
    \"unit\":\"\"
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
    \"unit\":\"\"
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
    \"unit\":\"\"
  }]}" | tr -d '\"'`
  
fi

echo "device $DEVICE"
echo "		name: SOIL-AREA-${1}"
echo "		with Soil Humidity Sensor displaying centibars from WM200"
echo "		with Soil Humidity Sensor displaying scaled value from WM200 real=x10"

echo "--> Make it LoRaWAN"
echo "	device id: 26011D$2"
curl -X POST "http://localhost/devices/${DEVICE}/meta" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d  "{\"codec\":\"application/x-xlpp\",\"lorawan\":{\"appSKey\":\"23158D3BBC31E6AF670D195B5AED5525\",\"devAddr\":\"26011D$2\",\"devEUI\":\"AA555A0026011D$2\",\"nwkSEncKey\":\"23158D3BBC31E6AF670D195B5AED5525\",\"profile\":\"WaziDev\"}}"

#####

NFILE=`ls -l $3.tensiometer.temperatureSensor_0.data_split* | wc -l`

for (( i = 0; i < $NFILE; i++ ))
do
	echo "--> Get temperatureSensor_0 sensor's values from $3.tensiometer.temperatureSensor_0.data_split_${i}.json"
	DATA=`cat $3.tensiometer.temperatureSensor_0.data_split_${i}.json`

	echo "--> Set sensor's values to device $DEVICE sensor temperatureSensor_0"
	curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_0/values" -H  "accept: application/json" -H "Authorization: Bearer $TOK" -d "$DATA"	
done

#####

## /home/pi/scripts/set_sensor_values.sh $3.tensiometer.temperatureSensor_1.data.json $DEVICE temperatureSensor_1
NFILE=`ls -l $3.tensiometer.temperatureSensor_1.data_split* | wc -l`

for (( i = 0; i < $NFILE; i++ ))
do
	echo "--> Get temperatureSensor_1 sensor's values from $3.tensiometer.temperatureSensor_1.data_split_${i}.json"
	DATA=`cat $3.tensiometer.temperatureSensor_1.data_split_${i}.json`

	echo "--> Set sensor's values to device $DEVICE sensor temperatureSensor_1"
	curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_1/values" -H  "accept: application/json" -H "Authorization: Bearer $TOK" -d "$DATA"	
done

#####

NFILE=`ls -l $3.tensiometer.temperatureSensor_2.data_split* | wc -l`

for (( i = 0; i < $NFILE; i++ ))
do
  echo "--> Get temperatureSensor_2 sensor's values from $3.tensiometer.temperatureSensor_2.data_split_${i}.json"
  DATA=`cat $3.tensiometer.temperatureSensor_2.data_split_${i}.json`

  echo "--> Set sensor's values to device $DEVICE sensor temperatureSensor_2"
  curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_2/values" -H  "accept: application/json" -H "Authorization: Bearer $TOK" -d "$DATA"  
done

#####

## /home/pi/scripts/set_sensor_values.sh $3.tensiometer.temperatureSensor_1.data.json $DEVICE temperatureSensor_1
NFILE=`ls -l $3.tensiometer.temperatureSensor_3.data_split* | wc -l`

for (( i = 0; i < $NFILE; i++ ))
do
  echo "--> Get temperatureSensor_3 sensor's values from $3.tensiometer.temperatureSensor_3.data_split_${i}.json"
  DATA=`cat $3.tensiometer.temperatureSensor_3.data_split_${i}.json`

  echo "--> Set sensor's values to device $DEVICE sensor temperatureSensor_3"
  curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_3/values" -H  "accept: application/json" -H "Authorization: Bearer $TOK" -d "$DATA"  
done

#####

echo "--> Create temperature sensor"

curl -X POST "http://localhost/devices/${DEVICE}/sensors" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
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
  \"unit\":\"\"
}"

echo "device $DEVICE"
echo "		with soil temperature displaying degree Celsius"

#####

##as we always create temperatureSensor_5, if there are no data from temperatureSensor_5 then we put value -99
if [ -f "$3.tensiometer.temperatureSensor_5.data.json" ]; then

	NFILE=`ls -l $3.tensiometer.temperatureSensor_5.data_split* | wc -l`

	for (( i = 0; i < $NFILE; i++ ))
	do
		echo "--> Get temperatureSensor_5 sensor's values from $3.tensiometer.temperatureSensor_5.data_split_${i}.json"
		DATA=`cat $3.tensiometer.temperatureSensor_5.data_split_${i}.json`

		echo "--> Set sensor's values to device $DEVICE sensor temperatureSensor_5"
		curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_5/values" -H  "accept: application/json" -H "Authorization: Bearer $TOK" -d "$DATA"	
	done
else 
  echo "no $3.tensiometer.temperatureSensor_5.data.json"
	echo "--> Add value -99"
	curl -X POST "http://localhost/devices/${DEVICE}/sensors/temperatureSensor_5/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":-99, \"time\":\"$DATE\"}"    
fi

#####

echo "--> Create voltage monitor sensor"

curl -X POST "http://localhost/devices/${DEVICE}/sensors" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{
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
  \"unit\":\"\"
}"

echo "device $DEVICE"
echo "		with voltage monitor displaying volt"

#####

#as we always create analogInput_6, if there are no data from analogInput_6 then we put value -1
if [ -f "$3.tensiometer.analogInput_6.data.json" ]; then

	NFILE=`ls -l $3.tensiometer.analogInput_6.data_split* | wc -l`

	for (( i = 0; i < $NFILE; i++ ))
	do
		echo "--> Get analogInput_6 sensor's values from $3.tensiometer.analogInput_6.data_split_${i}.json"
		DATA=`cat $3.tensiometer.analogInput_6.data_split_${i}.json`

		echo "--> Set sensor's values to device $DEVICE sensor analogInput_6"
		curl -X POST "http://localhost/devices/${DEVICE}/sensors/analogInput_6/values" -H  "accept: application/json" -H "Authorization: Bearer $TOK" -d "$DATA"	
	done
else 
  echo "no $3.tensiometer.analogInput_6.data.json"
	echo "--> Add value -1"
	curl -X POST "http://localhost/devices/${DEVICE}/sensors/analogInput_6/value" -H "accept: application/json" -H "Authorization: Bearer $TOK" -H  "Content-Type: application/json" -d "{\"value\":-1, \"time\":\"$DATE\"}"    
fi

#####

echo "Done"






