Guillaume's custom boot script for a range of devices
=====================================================

create-custom-multiple-range-iiwa
---------------------------------

This script takes two integer arguments:
    `sudo ./boot/create-custom-multiple-range-iiwa/intel-irris-auto-config.sh 3 9`
will for example generate the configuration (dashboard and iiwa) for three capacitive sensors (devAddr AA, AB, AC) and 9 watermark + temperature devices (devAddr B1, B2, ..., B9).

A default value (2 2) is chosen for a (first, bootstrap) argument-less configuration

