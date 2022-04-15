#first device on INTEL-IRRIS SD card image – capacitive sensor or tensiometer sensor
device_id_for_oled="62286d72f06c4c0001eba943"

#second device on INTEL-IRRIS SD card image – tensiometer sensor
#device_id2_for_oled="622a0c50f8f06600016be8fd"

#test device 
#device_id_for_oled="62225f4d491ae300015c7344"

get_sensor_type_from_local_database=True
get_value_index_from_local_database=False
set_value_index_in_local_database=True

sensor_type="capacitive"
sensor_model="SEN0308"

#sensor_type="tensiometer"
#sensor_model="WM200"

#comment for english
lang="fr"

capacitive_sensor_dry_max=800
capacitive_sensor_wet_max=0
capacitive_sensor_n_interval=6
capacitive_sensor_soil_condition=[]

if lang=="fr":
	capacitive_sensor_soil_condition.append('très sec')
	capacitive_sensor_soil_condition.append('sec')
	capacitive_sensor_soil_condition.append('sec-hum')
	capacitive_sensor_soil_condition.append('hum-sec')
	capacitive_sensor_soil_condition.append('hum')
	capacitive_sensor_soil_condition.append('très hum')
else:
	capacitive_sensor_soil_condition.append('very dry')
	capacitive_sensor_soil_condition.append('dry')
	capacitive_sensor_soil_condition.append('dry-wet')
	capacitive_sensor_soil_condition.append('wet-dry')
	capacitive_sensor_soil_condition.append('wet')
	capacitive_sensor_soil_condition.append('very wet')	

tensiometer_sensor_dry_max=0
tensiometer_sensor_wet_max=550
tensiometer_sensor_n_interval=5
tensiometer_sensor_soil_condition=[]
tensiometer_sensor_soil_condition.append('very dry')
tensiometer_sensor_soil_condition.append('dry')
tensiometer_sensor_soil_condition.append('dry-wet')
tensiometer_sensor_soil_condition.append('wet-dry')
tensiometer_sensor_soil_condition.append('wet')
tensiometer_sensor_soil_condition.append('very wet')