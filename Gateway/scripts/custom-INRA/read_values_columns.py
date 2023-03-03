import json
from datetime import datetime, timezone


EXPORT_JSON=True            # JSON.
EXPORT_SETTAT=True          # Settat's format
EXPORT_SINGLE_TIME=True     # Bondy's format.
EXPORT_GRAPHS_PANDAS=False  # Generates plots. Requires pandas and matplotlib Python libraries


f = open("extracted.json", "r")

############### CONVERSION extracted.json (blocks of JSON in a text file) into Python dict #########################
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

############### ENRICHED METADATA FROM UI #########################
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


data_dict={"sensortypes":sensortypes, "extracted_values":extracted_values}

if EXPORT_GRAPHS_PANDAS:
    import pandas as pd
    import matplotlib.pyplot as plt

    temperature_axed=False
    for device_id in extracted_values:
        device=extracted_values[len(extracted_values)-device_id-1]
        if len(device["temperatureSensor_5"])>0:
            df = pd.DataFrame(device["temperatureSensor_5"][1:])
            df['time']=pd.to_datetime(df['time'])
            # print(df['value'].max())
            if df['value'].max()>-30:
                # print(df.columns)
                df.rename(columns={'value': device["devName"]}, inplace=True)
                if not temperature_axed:
                    temperature_ax=df.plot(x='time',y=device["devName"],xlabel="Time",ylabel=sensortypes["capa"]["temperatureSensor_5"])
                    temperature_axed=True
                else:
                    df.plot(x='time',y=device["devName"],ax=temperature_ax,xlabel="Time")

    fig = plt.gcf()
    fig.set_size_inches(12,9)
    fig.savefig("temperatures.png",bbox_inches="tight")

    battery_axed=False
    for device_id in extracted_values:
        device=extracted_values[len(extracted_values)-device_id-1]
        if len(device["analogInput_6"])>0:
            df = pd.DataFrame(device["analogInput_6"][1:])
            df['time']=pd.to_datetime(df['time'])
            # print(df['value'].max())
            # if df['value'].max()>-30:
                # print(df.columns)
            df.rename(columns={'value': device["devName"]}, inplace=True)
            if not battery_axed:
                battery_ax=df.plot(x='time',y=device["devName"],xlabel="Time",ylabel=sensortypes["capa"]["analogInput_6"])
                battery_axed=True
            else:
                df.plot(x='time',y=device["devName"],ax=battery_ax,xlabel="Time")

    fig = plt.gcf()
    fig.set_size_inches(12,9)
    # fig.savefig('test2png.png', dpi=100)
    fig.savefig("batteries.png",bbox_inches="tight")

    SEN0308_axed=False
    for device_id in extracted_values:
        device=extracted_values[len(extracted_values)-device_id-1]
        if device["devAddr"][-2]=='A':
            if len(device["temperatureSensor_0"])>0:
                df = pd.DataFrame(device["temperatureSensor_0"][2:])
                df['time']=pd.to_datetime(df['time'])
                df.rename(columns={'value': device["devName"]}, inplace=True)
                if not SEN0308_axed:
                    SEN0308_ax=df.plot(x='time',y=device["devName"],xlabel="Time",ylabel=sensortypes["capa"]["temperatureSensor_0"])
                    SEN0308_axed=True
                else:
                    df.plot(x='time',y=device["devName"],ax=SEN0308_ax,xlabel="Time")
    fig = plt.gcf()
    fig.set_size_inches(12,9)
    fig.savefig("capas.png",bbox_inches="tight")

    WM200_cbars_axed=False
    for device_id in extracted_values:
        device=extracted_values[len(extracted_values)-device_id-1]
        if device["devAddr"][-2]=='B':
            if len(device["temperatureSensor_0"])>0:
                df = pd.DataFrame(device["temperatureSensor_0"][2:])
                df['time']=pd.to_datetime(df['time'])
                df.rename(columns={'value': device["devName"]}, inplace=True)
                if not WM200_cbars_axed:
                    WM200_cbars_ax=df.plot(x='time',y=device["devName"],xlabel="Time",ylabel=sensortypes["WM"]["temperatureSensor_0"])
                    WM200_cbars_axed=True
                else:
                    df.plot(x='time',y=device["devName"],ax=WM200_cbars_ax,xlabel="Time")
    fig = plt.gcf()
    fig.set_size_inches(12,9)
    fig.savefig("WM_cbars.png",bbox_inches="tight")

    WM200_realx10_axed=False
    for device_id in extracted_values:
        device=extracted_values[len(extracted_values)-device_id-1]
        if device["devAddr"][-2]=='B':
            if len(device["temperatureSensor_1"])>0:
                df = pd.DataFrame(device["temperatureSensor_1"][2:])
                df['time']=pd.to_datetime(df['time'])
                df.rename(columns={'value': device["devName"]}, inplace=True)
                if not WM200_realx10_axed:
                    WM200_realx10_ax=df.plot(x='time',y=device["devName"],xlabel="Time",ylabel=sensortypes["WM"]["temperatureSensor_1"])
                    WM200_realx10_axed=True
                else:
                    df.plot(x='time',y=device["devName"],ax=WM200_realx10_ax,xlabel="Time")
    fig = plt.gcf()
    fig.set_size_inches(12,9)
    fig.savefig("WM_WM200_realx10.png",bbox_inches="tight")
    #         # https://pandas.pydata.org/docs/reference/api/pandas.DataFrame.plot.html
    #         # In [12]: pd.to_datetime(pd.Series(['05/23/2005']), format="%m/%d/%Y")
    #     # plt.show()


# exporting to JSON
if EXPORT_JSON:
    json_dict = json.dumps(data_dict, indent=4)
    with open("full_JSON_export.json", "w") as outfile:
        outfile.write(json_dict)

if EXPORT_SINGLE_TIME:
    # exporting to Bondy aggregated CSV format
    with open('extracted_Bondy_aggregated.csv', 'w') as f:
        aggr_timestamps=[]

        f.write(',')
        for device_id in extracted_values:
            device=extracted_values[len(extracted_values)-device_id-1]
            aggr_timestamps+=[timeval["time"] for timeval in device["temperatureSensor_0"][2:]]
            f.write(device["devName"])
            for sensor in ["temperatureSensor_0","temperatureSensor_1","temperatureSensor_5","analogInput_6"]:
                if len(device[sensor])>0:
                    f.write(',')

        # sort list of timestamps
        aggr_timestamps = sorted(set(aggr_timestamps))

        f.write('\n,')

        for device_id in extracted_values:
            # for 
            device=extracted_values[len(extracted_values)-device_id-1]
            f.write(device["devID"])
            for sensor in ["temperatureSensor_0","temperatureSensor_1","temperatureSensor_5","analogInput_6"]:
                if len(device[sensor])>0:
                    f.write(',')

        f.write('\n,')

        for device_id in extracted_values:
            device=extracted_values[len(extracted_values)-device_id-1]
            for sensor in ["temperatureSensor_0","temperatureSensor_1","temperatureSensor_5","analogInput_6"]:
                if len(device[sensor])>0:
                    if device["devAddr"][-2]=='A':
                        f.write(sensortypes["capa"][sensor])
                    else:# B
                        f.write(sensortypes["WM"][sensor])
                    f.write(',')

        f.write('\n')


        for ordered_timestamp in aggr_timestamps:
            f.write(ordered_timestamp)
            f.write(',')

            for device_id in extracted_values:
                # for 
                device=extracted_values[len(extracted_values)-device_id-1]


                for sensor in ["temperatureSensor_0","temperatureSensor_1","temperatureSensor_5","analogInput_6"]:
                    if len(device[sensor])>0:
                        not_found=True
                        for timevalpair in device[sensor][2 if sensor[-1] in ["0","1"] else 1:]:
                            if timevalpair["time"]==ordered_timestamp:
                                f.write(str(timevalpair["value"]))
                                f.write(',')
                                not_found=False
                                break
                        if not_found:
                            f.write("n/a")
                            f.write(',')
            f.write('\n')
            





if EXPORT_SETTAT:
    # exporting to INRA January 23 CSV format
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
            if len(device["temperatureSensor_0"])>2:
                starttime=datetime.fromisoformat(device["temperatureSensor_0"][2]["time"])
            else:
                starttime=datetime.fromisoformat(device["temperatureSensor_0"][-1]["time"])

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
