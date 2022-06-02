#first device on INTEL-IRRIS SD card image – capacitive sensor or tensiometer sensor
device_id_for_oled='62286d72f06c4c0001eba943'

get_sensor_type_from_local_database=True
get_value_index_from_local_database=False
set_value_index_in_local_database=True
use_irrometer_interval_for_tensiometer=True
cyclic_show_all_device=True

sensor_type='capacitive'
sensor_model='SEN0308'

#sensor_type="tensiometer"
#sensor_model="WM200"

#change to 'en' for english
lang='fr'

capacitive_sensor_dry_max=800
capacitive_sensor_wet_max=0
capacitive_sensor_n_interval=6
capacitive_sensor_soil_condition=[]
#we adopt the following rule: 0:very dry; 1:dry; 2:dry-wet 3-wet-dry; 4-wet; 5-very wet

if lang=="fr":
	capacitive_sensor_soil_condition.append('très sec')
	capacitive_sensor_soil_condition.append('sec')
	capacitive_sensor_soil_condition.append('sec-hum')
	capacitive_sensor_soil_condition.append('hum-sec')
	capacitive_sensor_soil_condition.append('hum')
	capacitive_sensor_soil_condition.append('saturé')
else:
	capacitive_sensor_soil_condition.append('very dry')
	capacitive_sensor_soil_condition.append('dry')
	capacitive_sensor_soil_condition.append('dry-wet')
	capacitive_sensor_soil_condition.append('wet-dry')
	capacitive_sensor_soil_condition.append('wet')
	capacitive_sensor_soil_condition.append('saturated')	

tensiometer_sensor_dry_max=120
tensiometer_sensor_wet_max=0
tensiometer_sensor_n_interval=6
tensiometer_sensor_soil_condition=[]
#we adopt the following rule: 0:very dry; 1:dry; 2:dry-wet 3-wet-dry; 4-wet; 5-very wet

if lang=="fr":
	tensiometer_sensor_soil_condition.append('très sec')
	tensiometer_sensor_soil_condition.append('sec')
	tensiometer_sensor_soil_condition.append('sec-hum')
	tensiometer_sensor_soil_condition.append('hum-sec')
	tensiometer_sensor_soil_condition.append('hum')
	tensiometer_sensor_soil_condition.append('saturé')
else:
	tensiometer_sensor_soil_condition.append('very dry')
	tensiometer_sensor_soil_condition.append('dry')
	tensiometer_sensor_soil_condition.append('dry-wet')
	tensiometer_sensor_soil_condition.append('wet-dry')
	tensiometer_sensor_soil_condition.append('wet')
	tensiometer_sensor_soil_condition.append('saturated')	