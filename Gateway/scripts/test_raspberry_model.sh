#!/bin/bash

revision=`cat /proc/cpuinfo | grep "Revision" | cut -d ':' -f 2 | tr -d " \t\n\r"`

rev_hex="0x$revision"

echo "Revision code is $rev_hex"

#uuuuuuuuFMMMCCCCPPPPTTTTTTTTRRRR

type=$(( rev_hex & 0x00000FF0 ))

board=$(( type >> 4 ))

printf 'type code is (hex) %X\n' $(( type >> 4 ))
echo "type code is (dec) $board"

if [ "$board" = "0" ] || [ "$board" = "1" ] || [ "$board" = "2" ] || [ "$board" = "3" ]
	then
		echo "You have a Raspberry 1"
		echo ">RPI1/0"
elif [ "$board" = "4" ]
	then
		echo "You have a Raspberry 2"
		echo ">RPI2/3"
elif [ "$board" = "8" ]
	then
		echo "You have a Raspberry 3B"
		echo ">RPI2/3"
elif [ "$board" = "13" ] || [ "$board" = "14" ]
	then
		echo "You have a Raspberry 3B+/3A+"
		echo ">RPI2/3"
elif [ "$board" = "9" ]
	then
		echo "You have a Raspberry Zero"
		echo ">RPI1/0"
elif [ "$board" = "12" ]
	then
		echo "You have a Raspberry Zero W"
		echo ">RPI1/0"
elif [ "$board" = "17" ]
	then
		echo "You have a Raspberry 4B"
		echo ">RPI4"
elif [ "$board" = "20" ]
	then
		echo "You have a Raspberry Compute Module 4, using Raspberry 4 definition"
		echo ">RPI4"		
elif [ "$board" = "23" ]
	then
		echo "You have a Raspberry 5, using Raspberry 5 definition"
		echo ">RPI5"	
else
	echo "Don't know, sorry"
	echo ">NA"
fi





