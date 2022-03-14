"""
Based on example from Adafruit.

INTEL-IRRIS oled service. Author: C. Pham. Last update March 14th, 2022.

Called by /etc/systemd/system/intel-irris-oled-service.service

sudo systemctl enable intel-irris-oled-service.service

systemctl status intel-irris-oled-service.service
journalctl -u intel-irris-oled-service.service

"""

import sys
import time
#import subprocess
import os
#import os.path
import signal
import requests
from datetime import datetime
import random
import json

import board
import digitalio
from PIL import Image, ImageDraw, ImageFont
import adafruit_ssd1306

#-------------------------------------------------------------------------
# information related to the soil sensor device
#-------------------------------------------------------------------------

import key_device

#-------------------------------------------------------------------------
# OLED configuration
#-------------------------------------------------------------------------

# Define the Reset Pin
oled_reset = None

# Change these
# to the right size for your display!
WIDTH = 128
HEIGHT = 64  # Change to 64 if needed
BORDER = 2

# Use for I2C.
try:
	i2c = board.I2C()
except OSError:	
	print ('oled-service: error with I2C bus')
	sys.exit(1)
	#while True:
	#	time.sleep(1)

try:
	oled = adafruit_ssd1306.SSD1306_I2C(WIDTH, HEIGHT, i2c, addr=0x3C, reset=oled_reset)
except ValueError:	
	print ('oled-service: error with OLED address')
	sys.exit(2)
	#while True:
	#	time.sleep(1)
		
font_size = 10
font_saver_size = 14

# Load default font.
font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", font_size)
font_saver = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", font_saver_size)
#font = ImageFont.load_default()

# Create blank image for drawing.
# Make sure to create image with mode '1' for 1-bit color.
image = Image.new("1", (oled.width, oled.height))

# Get drawing object to draw on image.
draw = ImageDraw.Draw(image)
	
#-------------------------------------------------------------------------
# oled-service configuration
#-------------------------------------------------------------------------

#duration of screen saver mode
_oled_display_timer=120
#duration of the full information screen
_oled_display_duration=6

if key_device.sensor_type=='capacitive':
	last_raw_value=key_device.capacitive_sensor_dry_max
else:
	last_raw_value=key_device.tensiometer_sensor_wet_max	
	
last_raw_value_minutes=0
last_raw_value_days=0

capacitive_soil_condition=key_device.capacitive_sensor_soil_condition[key_device.capacitive_sensor_n_interval-1]
tensiometer_soil_condition=key_device.tensiometer_sensor_soil_condition[key_device.tensiometer_sensor_n_interval-1]

#-------------------------------------------------------------------------
# WaziGate API configuration
#-------------------------------------------------------------------------

#common header for requests
WaziGate_headers = {'accept':'application/json','content-type':'application/json'}

#-------------------------------------------------------------------------
# Interruption handler
#-------------------------------------------------------------------------

def signal_handler(sig, frame):
	print('You pressed Ctrl+C!')
	# Clear display.
	oled.fill(0)
	oled.show()	
	sys.exit(3)

signal.signal(signal.SIGINT, signal_handler)
signal.signal(signal.SIGTERM, signal_handler)

#-------------------------------------------------------------------------
# display at random position the summary, avoiding fix position on the OLED
#-------------------------------------------------------------------------

def screen_saver(duration):
	
	# Clear display.
	oled.fill(0)
	oled.show()
		
	min_x=2*BORDER
	max_x=oled.width - 5*font_saver_size
	min_y=2*BORDER
	max_y=oled.height - 3*font_saver_size
	
	# Get drawing object to draw on image.
	#draw = ImageDraw.Draw(image)	
	
	start_time = time.time()
	end_time=start_time
	
	while (end_time-start_time < duration):
		# Draw a back background
		draw.rectangle((0, 0, oled.width, oled.height), outline=0, fill=0)	
		x=random.randint(min_x, max_x)
		y=random.randint(min_y, max_y)
		
		draw.text((x, y), str(last_raw_value), font=font_saver, fill=255)
		
		if last_raw_value_days:
			text_to_display=str(last_raw_value_days)+" days"
		else:		
			text_to_display=str(last_raw_value_minutes)+" mins"		
		draw.text((x, y+font_saver_size), text_to_display, font=font_saver, fill=255)
	
		if key_device.sensor_type=='capacitive':
			draw.text((x, y+2*font_saver_size), capacitive_soil_condition, font=font_saver, fill=255)
		else:
			draw.text((x, y+2*font_saver_size), tensiometer_soil_condition, font=font_saver, fill=255)	
				
		# Display image
		oled.image(image)
		oled.show()		
		time.sleep(2)
		oled.fill(0)
		oled.show()
		end_time = time.time()
		
#--------------------------------------------------------------------------
#get last stored value for a given device id: key_device.device_id_for_oled
#--------------------------------------------------------------------------		
						
def get_last_raw_value():
	WaziGate_url='http://localhost/devices/'+key_device.device_id_for_oled+'/sensors/temperatureSensor_0'
	try:
		response = requests.get(WaziGate_url, headers=WaziGate_headers, timeout=30)
		print ('oled-service: returned msg from server is '),
		print (response.status_code)
		print (response.reason)
		
		if 200 <= response.status_code < 300:
			print ('oled-service: GET success')
			print (response.text)
			device_json=json.loads(response.text)
			global last_raw_value
			last_raw_value=device_json["value"]
			print(last_raw_value)

			fmt1 = '%Y-%m-%dT%H:%M:%S.%f'
			fmt2 = '%Y-%m-%dT%H:%M:%S.%fZ'
			d1 = datetime.strptime(datetime.utcnow().isoformat(), fmt1)
			#here time stored in database is in UTC
			d2 = datetime.strptime(device_json["time"], fmt2)

			diff = d1-d2
			global last_raw_value_days
			last_raw_value_days=diff.days
			
			global last_raw_value_minutes
			# convert days to minutes
			last_raw_value_minutes = int(diff.days * 24 * 60 + (diff.seconds/60))
			
			print (str(last_raw_value_minutes)+"mins")
			
		else:
			print ('oled-service: bad request')
			print (response.text)			
			
	except requests.exceptions.RequestException as e:
		print (e)
		print ('oled-service: requests command failed')

#--------------------------------------------------------------------------
#determine the soil condition string indication for capacitive
#--------------------------------------------------------------------------

def get_capacitive_soil_condition(raw_value):
	value_interval=int(key_device.capacitive_sensor_dry_max/key_device.capacitive_sensor_n_interval)
	value_index=int(raw_value/value_interval)
	global capacitive_soil_condition
	capacitive_soil_condition=key_device.capacitive_sensor_soil_condition[value_index]

#--------------------------------------------------------------------------
#determine the soil condition string indication for tensiometer
#--------------------------------------------------------------------------
	
def get_tensiometer_soil_condition(raw_value): 
	value_interval=int(key_device.tensiometer_sensor_wet_max/key_device.tensiometer_sensor_n_interval)
	value_index=int(raw_value/value_interval)
	global tensiometer_soil_condition
	tensiometer_soil_condition=key_device.tensiometer_sensor_soil_condition[value_index]	
										
#------------------------------------------------------------
#main loop
#------------------------------------------------------------
	
while True:

	# Clear display.
	oled.fill(0)
	oled.show()

	# Draw a white background
	draw.rectangle((0, 0, oled.width, oled.height), outline=255, fill=255)

	# Draw a smaller inner rectangle
	draw.rectangle(
			(BORDER, BORDER, oled.width - BORDER - 1, oled.height - BORDER - 1),
			outline=0,
			fill=0,
	)

	padding = -2
	top = padding
	bottom = oled.height-padding
	# Move left to right keeping track of the current x position for drawing shapes.
	x = 2*BORDER

	#cmd= "date +%Y-%m-%dT%H:%M:%S"
	#date = subprocess.check_output(cmd, shell = True )

	# Write lines of text.
	ttop=top
	ttop=ttop+font_size

	draw.text((x, ttop), "INTEL-IRRIS WaziGate", font=font, fill=255)
	ttop=ttop+font_size

	text_to_display=key_device.sensor_model+" "+key_device.sensor_type
	draw.text((x, ttop), text_to_display, font=font, fill=255)
	ttop=ttop+font_size

	get_last_raw_value()
	
	if last_raw_value_days:
		text_to_display=str(last_raw_value_days)+"days ago"
	else:		
		text_to_display=str(last_raw_value_minutes)+"mins ago"
	draw.text((x, ttop), text_to_display, font=font, fill=255)
	ttop=ttop+font_size
	
	text_to_display="raw: "+str(last_raw_value)
	draw.text((x, ttop), text_to_display, font=font, fill=255)
	ttop=ttop+font_size
	
	if key_device.sensor_type=='capacitive':
		get_capacitive_soil_condition(last_raw_value)
		text_to_display="soil is "+str(capacitive_soil_condition)
	else:
		get_tensiometer_soil_condition(last_raw_value)
		text_to_display="soil is "+str(tensiometer_soil_condition)	

	draw.text((x, ttop), text_to_display, font=font, fill=255)
	ttop=ttop+font_size

	# Display image
	oled.image(image)
	oled.show()
	
	time.sleep(_oled_display_duration)
	
	screen_saver(_oled_display_timer)	
