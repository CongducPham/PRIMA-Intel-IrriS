Tool scripts to backup and restore device sensor's data
=======================================================

You can use this folder for your device sensor backup/restore tasks.

Backup: to backup a given device
------

**For a capacitive sensor device:**

	> /home/pi/scripts/backup_capacitive_device_sensor_values.sh <device_id>
	
Example:

	> /home/pi/scripts/backup_capacitive_device_sensor_values.sh 62c7c657127dbd00011540a6	

Produces:

- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_0.data.json
- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_5.data.json, if soil temperature connected
- 62c7c657127dbd00011540a6.capacitive.analogInput_6.data.json


**For a tensiometer sensor device:**

	> /home/pi/scripts/backup_tensiometer_device_sensor_values.sh <device_id>
	
Example:

	> /home/pi/scripts/backup_tensiometer_device_sensor_values.sh 62c7c657127dbd00011540a6	

Produces:

- 62c7c657127dbd00011540a6.tensiometer.temperatureSensor_0.data.json
- 62c7c657127dbd00011540a6.tensiometer.temperatureSensor_1.data.json
- 62c7c657127dbd00011540a6.tensiometer.temperatureSensor_5.data.json, if soil temperature connected
- 62c7c657127dbd00011540a6.tensiometer.analogInput_6.data.json

Restore: to restore to a new device
-------

The restore process **automatically creates a new device** where sensor's data previously backup will be restored.


**For a capacitive sensor device:**

	> /home/pi/scripts/restore_capacitive_device_sensor_values.sh <new_device_index> <new_device_addr> <from_device_id>
	
Example:

	> /home/pi/scripts/restore_capacitive_device_sensor_values.sh 1 AA 62c7c657127dbd00011540a6 	

Creates a new & empty capacitive sensor device with:

- device's name: SOIL-AREA-1
- device's address: 26011DAA

Restore sensor's data from:

- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_0.data.json
- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_5.data.json, if file exists
- 62c7c657127dbd00011540a6.capacitive.analogInput_6.data.json

**For a tensiometer sensor device:**

	> /home/pi/scripts/restore_tensiometer_device_sensor_values.sh <new_device_index> <new_device_addr> <from_device_id>
	
Example:

	> /home/pi/scripts/backup_tensiometer_device_sensor_values.sh 1 AA 62c7c657127dbd00011540a6	

Creates a new & empty tensiometer sensor device:

- device's name: SOIL-AREA-1
- device's address: 26011DAA

Restore sensor's data from:

- 62c7c657127dbd00011540a6.tensiometer.temperatureSensor_0.data.json
- 62c7c657127dbd00011540a6.tensiometer.temperatureSensor_1.data.json
- 62c7c657127dbd00011540a6.tensiometer.temperatureSensor_5.data.json, if file exists
- 62c7c657127dbd00011540a6.tensiometer.analogInput_6.data.json


Enjoy!
C. Pham
Coordinator of PRIMA Intel-IrriS

