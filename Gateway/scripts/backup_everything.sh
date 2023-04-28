#!/bin/bash

# Ex: backup_starter_kit.sh
# this script backup the starter-kit default device to sensor-backup folder
# if an argument is provided, the scripts will try to copy backup files to USB drive
# normally USB drive is /dev/sda1
# Ex: backup_starter_kit.sh tousbdrive

cd /home/pi/sensor-backup

echo "removing sensor-backup.log file"
rm -rf sensor-backup.log

echo `date` >> sensor-backup.log

echo "removing all split files" >> sensor-backup.log
rm -rf *split*

TOK=`curl -X POST "http://localhost/auth/token" -H  "accept: application/json" -H  "Content-Type: application/json" -d "{\"username\":\"admin\",\"password\":\"loragateway\"}" | tr -d '\"'`

DEVICES=`curl -X GET "http://localhost/devices" -H  "accept: application/json"`
NDEVICE=`echo $DEVICES | jq '. | length'`
(( NDEVICE-- ))
while [ $NDEVICE -gt 0 ]
do
  DEVICE=`echo $DEVICES | jq ".[${NDEVICE}].id"  | tr -d '\"'`
  # DEVICE=`curl -X GET "http://localhost/devices" -H  "accept: application/json" | jq ".[$NDEVICE].id" | tr -d '\"'`
  sizeDEVICE=${#DEVICE} 
  #we do not want to backup a gateway as it is also considered as a device
  if [ $sizeDEVICE -gt 16 ]
  then
	# - show_device_by_name.sh SOIL-AREA-1 sensors[0].meta.kind
	DEVTYPE=`echo $DEVICES | jq ".[${NDEVICE}].sensors[0].meta.type"  | tr -d '\"'`

	echo "backup $DEVTYPE device $DEVICE" >> sensor-backup.log
    if [ $DEVTYPE == 'capacitive' ]
    then
		/home/pi/scripts/backup_capacitive_device_sensor_values.sh $DEVICE
    elif [ $DEVTYPE == 'tensiometer' ]
    then
		/home/pi/scripts/backup_tensiometer_device_sensor_values.sh $DEVICE
	fi
  fi      
  (( NDEVICE-- ))
done

#only if one argument is provided
#Ex: backup_starter_kit.sh tousbdrive
if [ $# -eq 1 ]
then
	echo "mounting USB drive to /media for pi user" >> sensor-backup.log
	sudo mount -o uid=1000,gid=1000 /dev/sda1 /media
	MOUNT_RET_CODE=$?
	echo "mount return code is $MOUNT_RET_CODE" >> sensor-backup.log
	if [ $MOUNT_RET_CODE -eq 0 ]
	then
		sleep 1

		NDEVICE=`echo $DEVICES | jq '. | length'`
		(( NDEVICE-- ))
		while [ $NDEVICE -gt 0 ]
		do
		  DEVICE=`echo $DEVICES | jq ".[${NDEVICE}].id"  | tr -d '\"'`
		  sizeDEVICE=${#DEVICE} 
		  #we do not want to backup a gateway as it is also considered as a device
		  if [ $sizeDEVICE -gt 16 ]
		  then
			echo "copy device $DEVICE backup file to USB drive" >> sensor-backup.log
			cp $DEVICE* /media
		  fi      
		  (( NDEVICE-- ))
		done
		echo "unmounting USB drive at /media" >> sensor-backup.log
	else
		echo "could not mount /dev/sda1 to /media" >> sensor-backup.log
		echo "trying to umount" >> sensor-backup.log
	fi
	sudo umount /media	
fi