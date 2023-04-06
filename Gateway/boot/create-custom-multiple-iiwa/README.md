Guillaume's custom boot script for a set of devices
===================================================

create-custom-multiple-iiwa (still under testing)
-------------------------------------------------

This script takes a sequence of argument triplets:
    `sudo ./boot/create-custom-multiple-iiwa/intel-irris-auto-config.sh C 1 AE WT 8 B4`
will for example generate the configuration (dashboard and iiwa) for a capacitive device (devAddr AE) and a watermark + temperature device (devAddr B4).
For a device with two tensiometers and temperature, type 2WT.

A default value (C 1 AA WT 2 B1) is chosen for a (first, bootstrap) argument-less configuration
A list of bootstrap values may also be prepared in `/boot/devices.txt`.

