Tool scripts to backup and restore device sensor's data
=======================================================

You can use this folder for your device sensor backup/restore tasks. The scripts are executed in **command line mode** on the WaziGate.

Sections
------
**Backup: to backup a given device**
**Restore: to restore to a new device**
**Restore: to restore to a specific device id**
**To copy backup file stored on WaziGate to host computer**
**Sensor data samples**
**Full Manual Backup of your configured devices**
**Periodic and automatic backup of your configured devices**
**Full Manual Restore of some data and configurations**
**Use case: switch current values to a new gateway**

Backup: to backup a given device
------

**For a capacitive sensor device:**

	> cd sensor_backup
	> /home/pi/scripts/backup_capacitive_device_sensor_values.sh <device_id>
	
Example:

	> /home/pi/scripts/backup_capacitive_device_sensor_values.sh 62c7c657127dbd00011540a6	

Produces:

- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_0.data.json
- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_5.data.json, if soil temperature connected
- 62c7c657127dbd00011540a6.capacitive.analogInput_6.data.json

Then, each sensor data file (e.g. temperatureSensor_0.data.json) is further split into several files of 1000 lines:

- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_0.data_split_0.json
- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_0.data_split_1.json
- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_0.data_split_2.json
- ...

**For a (2-)tensiometer sensor device:**

	> cd sensor_backup
	> /home/pi/scripts/backup_tensiometer_device_sensor_values.sh <device_id>
	
Example:

	> /home/pi/scripts/backup_tensiometer_device_sensor_values.sh 62c7c657127dbd0001154bbc	

Produces:

- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_0.data.json
- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_1.data.json
- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_2.data.json, if a second tensiometer is connected
- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_3.data.json, if a second tensiometer is connected
- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_5.data.json, if soil temperature connected
- 62c7c657127dbd0001154bbc.tensiometer.analogInput_6.data.json

Then, each sensor data file (e.g. temperatureSensor_0.data.json) is further split into several files of 1000 lines:

- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_0.data_split_0.json
- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_0.data_split_1.json
- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_0.data_split_2.json
- ...

Restore: to restore to a new device
-------

The default behavior is to **automatically creates a new device** where sensor's data previously backup will be restored.

**For a capacitive sensor device:**

	> cd sensor_backup
	> /home/pi/scripts/restore_capacitive_device_sensor_values.sh <new_device_index> <new_device_addr> <from_device_id>
	
Example:

	> /home/pi/scripts/restore_capacitive_device_sensor_values.sh 1 AA 62c7c657127dbd00011540a6 	

Creates a new & empty capacitive sensor device with:

- device's name: SOIL-AREA-1
- device's address: 26011DAA

Restore sensor's data from the various split files:

- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_0.data_split_X.json
- 62c7c657127dbd00011540a6.capacitive.temperatureSensor_5.data_split_X.json, if file exists
- 62c7c657127dbd00011540a6.capacitive.analogInput_6.data_split_X.json

**For a tensiometer sensor device:**

	> /home/pi/scripts/restore_tensiometer_device_sensor_values.sh <new_device_index> <new_device_addr> <from_device_id>
	
Example:

	> cd sensor_backup
	> /home/pi/scripts/restore_tensiometer_device_sensor_values.sh 2 B1 62c7c657127dbd0001154bbc	

Creates a new & empty tensiometer sensor device:

- device's name: SOIL-AREA-2
- device's address: 26011DB1

Restore sensor's data from:

- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_0.data_split_X.json
- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_1.data_split_X.json
- 62c7c657127dbd0001154bbc.tensiometer.temperatureSensor_5.data_split_X.json, if file exists
- 62c7c657127dbd0001154bbc.tensiometer.analogInput_6.data_split_X.json

If you want a fresh start, you can delete all devices before you restore from backup files:

	> /home/pi/scripts/delete_all_devices.sh
	
**For a 2-tensiometer sensor device:**
Idem:

	> /home/pi/scripts/restore_2-tensiometer_device_sensor_values.sh <new_device_index> <new_device_addr> <from_device_id>

**If you want your new devices to be automatically included into IIWA and HA**, you must run `add_to_iiwa_ha_starterkit.sh` after the restoration process:

	> /home/pi/scripts/add_to_iiwa_ha_starterkit.sh

Note: this will only work for 2 devices composing a starter kit. See later for IIWA for more complex settings.

Restore: to restore to a specific device id
-------

For both capacitive and tensiometer, it is possible to indicate a specific device id so that the restore script will create a device with the indicated id. This may be useful if your backup devices are also on the WaziCloud and you want to continue synchronisation with the same device id. In order to do so, you need to add a parameter indicating the device id you want to use. It can be the same than the one you backup from:

	> /home/pi/scripts/restore_tensiometer_device_sensor_values.sh <new_device_index> <new_device_addr> <from_device_id> <new_device_id>

Example:

	> cd sensor_backup
	> /home/pi/scripts/restore_tensiometer_device_sensor_values.sh 2 B1 62c7c657127dbd0001154bbc 62c7c657127dbd0001154bbc
	
**However, make sure that on the WaziGate you are restoring the sensor data, there are no conflicting devices with same ids or same LoRaWAN DevAddr.** Therefore, it can be necessary to delete all devices before you restore from backup files.

	> /home/pi/scripts/delete_all_devices.sh
	
or, if you know which one you need to delete:

	> /home/pi/scripts/delete_device.sh 62c7c657127dbd0001154bbc

To copy backup file stored on WaziGate to host computer
-------

On host computer:

	> scp pi@192.168.3.5:/home/pi/sensor-backup/*.json .

Replace `192.168.3.5` by the IP address assigned to the WaziGate. If you are connecting to the WaziGate by the WaziGate's WiFi, use the static `10.42.0.1` IP address. You can also use an FTP software such as `FileZilla` and to open an SFTP connection using port 22. Remember user is `pi` and password is `loragateway`.

Sensor data samples
-------

The provided sensor data example samples in the `example` folder are taken from real INTEL-IRRIS devices deployed from June to December 2022 during the summer heat wave. There were no supplied water apart from rain. The soil type is mostly clay. There are one capacitive and one tensiometer INTEL-IRRIS devices close to each other. The tensiometer is buried at about 30cm deep.

You can restore these sensor data example samples on the INTEL-IRRIS WaziGate as follows (assuming you have the default starter-kit configuration where SOIL-AREA-1 and SOIL-AREA-2 are already defined):

	> cd sensor-backup
	> /home/pi/scripts/restore_capacitive_device_sensor_values.sh 3 AB 63a7191a68f3190886639cc7
	> /home/pi/scripts/restore_tensiometer_device_sensor_values.sh 4 B2 63a71aa368f319088663dc85

These commands will create 2 new devices, a capacitive as SOIL-AREA-3 and a tensiometer as SOIL-AREA-4, with addresses 26011DAB and 26011DB2, that will come in addition to the 2 default devices configured with the starter-kit. If you want to replace the default devices in order to be able to receive real sensor data from your devices, first delete all devices then run the above scripts by replacing `3 AB` and `4 B2` by respectively `1 AA` and `2 B1`.

Full Manual Backup of your configured devices
-------

In the case you set up a WaziGate with a non-standard configuration, i.e. with devices and sensors that difer from the starter kit scenario, and/or with specific IIWA configurations, you can manually backup everything on the GW, say in folder sensor-backup, like this:

	> cd sensor-backup
	> /home/pi/scripts/backup_everything.sh 

This script will list all the configured sensors and use the individual backup scripts on them. It also calls the IIWA REST scripts to get and store their IIWA configurations. 

It can be convenient to backup these data outside the GW, e.g. on a USB stick.  
Just plug a USB stick into the RaspberryPi and do the following:

	> /home/pi/scripts/backup_everything.sh tousbdrive
The script then mount the USB stick, copy the backup files on it, and unmount it.

Periodic and automatic backup of your configured devices
-------

In current version, the WaziGate periodically backups the configured devices data and IIWA configurations to folder `~/sensor-backup` and to a USB stick, in order to have an external regular backup. By default the backup will be realized every 6 hours. You can remove the USB stick at any time and plug it on your laptop/computer if needed. You can also re-plug the USB stick to the RaspberryPi at any time. The produced backup files are the same than what has been described previously.

The period of the backup procedure is available and may be modified here:

	> more /home/pi/scripts/crontab.pi

Note that if there is no USB stick attached to the RaspberryPi, the periodic backup files are still stored in `/home/pi/sensor-backup` folder. Note that there is no historical backup: the backup comprises all the available values and the newest backup overwrites any previous one.


Full Manual Restore of some data and configurations
-------

The restoration of the device from backup files (from USB stick or backup folder) can be done manually: 

	> cd sensor-backup
	> /home/pi/scripts/restore_everything.sh

Note: this script will delete all current devices and all specific IIWA configurations before restoring the backup ones with their backup IIWA configurations.

Use case: switch current values to a new gateway
-------

Let's take this example: you want to set up a new gateway without losing the history of all the data of your current gateway. 

First, on the current Gateway, plug a USB stick, go SSH, and do the following:

	> cd sensor-backup
	> /home/pi/scripts/backup_everything.sh tousbdrive

Remove the USB stick, plug it to the new gateway, go SSH there and do:

	> /home/pi/scripts/restore_everything.sh fromusbdrive




Enjoy!
C. Pham
Coordinator of PRIMA Intel-IrriS

G. Gaillard
Postdoc at UPPA