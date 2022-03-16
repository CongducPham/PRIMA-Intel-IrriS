#first device on INTEL-IRRIS SD card image – capacitive sensor
device_id_for_oled="62286d72f06c4c0001eba943"

#second device on INTEL-IRRIS SD card image – tensiometer sensor
#device_id_for_oled="622a0c50f8f06600016be8fd"

#test device 
#device_id_for_oled="62225f4d491ae300015c7344"

sensor_type="capacitive"
sensor_model="SEN0308"

#sensor_type="tensiometer"
#sensor_model="WMS200"

capacitive_sensor_dry_max=780
capacitive_sensor_wet_max=0
capacitive_sensor_n_interval=4
capacitive_sensor_soil_condition=[]
capacitive_sensor_soil_condition.append('very wet')
capacitive_sensor_soil_condition.append('wet')
capacitive_sensor_soil_condition.append('dry')
capacitive_sensor_soil_condition.append('very dry')

tensiometer_sensor_dry_max=0
tensiometer_sensor_wet_max=550
tensiometer_sensor_n_interval=4
tensiometer_sensor_soil_condition=[]
tensiometer_sensor_soil_condition.append('very wet')
tensiometer_sensor_soil_condition.append('wet')
tensiometer_sensor_soil_condition.append('dry')
tensiometer_sensor_soil_condition.append('very dry')