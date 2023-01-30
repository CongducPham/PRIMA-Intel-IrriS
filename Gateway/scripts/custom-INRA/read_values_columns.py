import json
from datetime import datetime, timezone

f = open("extracted.json", "r")

extracted_values={}
current_line=""
for l in f.readlines():
    if l[0]=='S':
        if len(current_line)>0:
            arrays=current_line.split(']')[:-1]
            nar=[]
            for a in arrays: 
                try:
                    nar.append(json.loads(a+']'))
                except:
                    print("Problem with JSON loads:")
                    print(a)
                    print("Skipping that line")
            arrays=nar
            # arrays=[json.loads(a+']') for a in arrays  ]
            extracted_values[len(extracted_values)-1]["temperatureSensor_0"]=arrays[0]
            extracted_values[len(extracted_values)-1]["temperatureSensor_1"]=arrays[1]
            extracted_values[len(extracted_values)-1]["temperatureSensor_5"]=arrays[2]
            extracted_values[len(extracted_values)-1]["analogInput_6"]=arrays[3]
            # print(0/0)
        # print(l.replace('\n','').split(":"))
        sensor=l.replace('\n','').split(":")
        extracted_values[len(extracted_values)]={
            "devName":sensor[0],
            "devAddr":sensor[1],
            "devID":sensor[2]
        }
        current_line=""
    else:
        current_line+=l[:-1]
if len(current_line)>0:
    arrays=current_line.split(']')[:-1]
    nar=[]
    for a in arrays: 
        try:
            nar.append(json.loads(a+']'))
        except:
            print("Problem with JSON loads:")
            print(a)
            print("Skipping that line")
    arrays=nar    
    # arrays=[json.loads(a+']') for a in arrays]
    # print(arrays)
    extracted_values[len(extracted_values)-1]["temperatureSensor_0"]=arrays[0]
    extracted_values[len(extracted_values)-1]["temperatureSensor_1"]=arrays[1]
    extracted_values[len(extracted_values)-1]["temperatureSensor_5"]=arrays[2]
    extracted_values[len(extracted_values)-1]["analogInput_6"]=arrays[3]


f.close()

# print(extracted_values[0])

sensortypes={
    "capa":{
        "temperatureSensor_0":"Soil Humidity Sensor/Raw value from SEN0308",
        "temperatureSensor_1":"garbage",
        "temperatureSensor_5":"Soil Temperature Sensor/degree Celsius",
        "analogInput_6":"Battery voltage/volt"
    },
    "WM":{
        "temperatureSensor_0":"Soil Humidity Sensor/centibars from WM200",
        "temperatureSensor_1":"Soil Humidity Sensor/scaled value from WM200 real=x10",
        "temperatureSensor_5":"Soil Temperature Sensor/degree Celsius",
        "analogInput_6":"Battery voltage/volt"
    }
}
# Soil Humidity Sensor/Raw value from SEN0308 (temperatureSensor_0)
# Soil Humidity Sensor/centibars from WM200 (temperatureSensor_0)
# Soil Humidity Sensor/scaled value from WM200 real=x10 (temperatureSensor_1)
# Soil Temperature Sensor/degree Celsius (temperatureSensor_5)
# Battery voltage/volt, low battery when lower than 2.85V (analogInput_6)

with open('extracted_2.csv', 'w') as f:
    for device_id in extracted_values:
        device=extracted_values[len(extracted_values)-device_id-1]

        f.write(device["devName"])
        f.write(': ')
        f.write(device["devAddr"])
        f.write(': ')
        if device["devAddr"][-2]=='A':
            f.write("Capacitif")
        else:
            f.write("Watermark + ST")
        f.write(': ')
        f.write(device["devID"])
        f.write(',')

        f.write("Date (aaaa-mm-jj)")
        f.write(',')
        for timevalpair in device["temperatureSensor_0"][2:-1]:
            f.write(datetime.fromisoformat(timevalpair["time"]).strftime("%Y-%m-%d"))
            f.write(',')
        f.write(datetime.fromisoformat(device["temperatureSensor_0"][-1]["time"]).strftime("%Y-%m-%d"))
        f.write('\n,')

        f.write("heure-min-sec")
        f.write(',')
        for timevalpair in device["temperatureSensor_0"][2:-1]:
            f.write(datetime.fromisoformat(timevalpair["time"]).strftime("%H-%M-%S"))
            f.write(',')
        f.write(datetime.fromisoformat(device["temperatureSensor_0"][-1]["time"]).strftime("%H-%M-%S"))
        f.write('\n,')

        f.write("heure calendaire")
        f.write(',')
        for timevalpair in device["temperatureSensor_0"][2:-1]:
            hcal=int(datetime.fromisoformat(timevalpair["time"]).strftime("%H"))
            mcal=int(datetime.fromisoformat(timevalpair["time"]).strftime("%M"))
            if mcal>29:
                hcal+=1
            f.write(str(hcal))
            f.write(',')
        hcal=int(datetime.fromisoformat(device["temperatureSensor_0"][-1]["time"]).strftime("%H"))
        mcal=int(datetime.fromisoformat(device["temperatureSensor_0"][-1]["time"]).strftime("%M"))
        if mcal>29:
            hcal+=1
        f.write(str(hcal))
        f.write('\n,')        

        f.write("secondes depuis le début")
        f.write(',')
        starttime=datetime.fromisoformat(device["temperatureSensor_0"][2]["time"])        
        for timevalpair in device["temperatureSensor_0"][2:-1]:
            f.write(str(int((datetime.fromisoformat(timevalpair["time"])-starttime).total_seconds())))
            f.write(',')
        f.write(str(int((datetime.fromisoformat(device["temperatureSensor_0"][-1]["time"])-starttime).total_seconds())))
        f.write('\n,')

        for sensor in ["temperatureSensor_0","temperatureSensor_1"]:
            if len(device[sensor])>0:
                if device["devAddr"][-2]=='A':
                    f.write(sensortypes["capa"][sensor])
                else:# B
                    f.write(sensortypes["WM"][sensor])
                f.write(',')
                for timevalpair in device[sensor][2:-1]:
                    f.write(str(timevalpair["value"]))
                    f.write(',')
                f.write(str(device[sensor][-1]["value"]))
                f.write('\n,')
        for sensor in ["temperatureSensor_5","analogInput_6"]:
            if len(device[sensor])>0:
                if device["devAddr"][-2]=='A':
                    f.write(sensortypes["capa"][sensor])
                else:# B
                    f.write(sensortypes["WM"][sensor])
                f.write(',')
                for timevalpair in device[sensor][1:-1]:
                    f.write(str(timevalpair["value"]))
                    f.write(',')
                f.write(str(device[sensor][-1]["value"]))
                if sensor=="analogInput_6":
                    f.write('\n\n')
                else:
                    f.write('\n,')

# import csv
# # from itertools import izip
# a = zip(*csv.reader(open("extracted_2.csv", "r")))
# csv.writer(open("extracted_final_columns.csv", "w")).writerows(a)

# from csv import reader, writer 
# with open('extracted_2.csv') as f, open('extracted_final_columns.csv', 'w') as fw: 
#     writer(fw, delimiter=',').writerows(zip(*reader(f, delimiter=',')))

# todo: replace by sensor types from IIWA
# todo: timestamps read 

# strtime="2023-01-13T07:00:00+01:00"
# print(datetime.fromisoformat(strtime).strftime("%Y-%m-%d-%H-%M-%S"))
