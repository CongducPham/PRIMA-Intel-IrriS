import json
import pandas as pd
import matplotlib.pyplot as plt

with open('full_JSON_export.json') as f:
   data_dict = json.load(f)

sensortypes=data_dict["sensortypes"]
extracted_values=data_dict["extracted_values"]


# Temperature plot 
temperature_axed=False
for device_id in extracted_values:
    device=extracted_values[str(len(extracted_values)-int(device_id)-1)]
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
    device=extracted_values[str(len(extracted_values)-int(device_id)-1)]
    if len(device["analogInput_6"])>0:
        df = pd.DataFrame(device["analogInput_6"][1:])
        df['time']=pd.to_datetime(df['time'])
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
    device=extracted_values[str(len(extracted_values)-int(device_id)-1)]
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
    device=extracted_values[str(len(extracted_values)-int(device_id)-1)]
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
    device=extracted_values[str(len(extracted_values)-int(device_id)-1)]
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

WM200_cbars_axed=False
for device_id in extracted_values:
    device=extracted_values[str(len(extracted_values)-int(device_id)-1)]
    if device["devAddr"][-2]=='B':
        if len(device["temperatureSensor_2"])>0:
            df = pd.DataFrame(device["temperatureSensor_2"][2:])
            df['time']=pd.to_datetime(df['time'])
            df.rename(columns={'value': device["devName"]}, inplace=True)
            if not WM200_cbars_axed:
                WM200_cbars_ax=df.plot(x='time',y=device["devName"],xlabel="Time",ylabel=sensortypes["WM"]["temperatureSensor_2"])
                WM200_cbars_axed=True
            else:
                df.plot(x='time',y=device["devName"],ax=WM200_cbars_ax,xlabel="Time")
fig = plt.gcf()
fig.set_size_inches(12,9)
fig.savefig("WM2_WM_cbars.png",bbox_inches="tight")

WM200_realx10_axed=False
for device_id in extracted_values:
    device=extracted_values[str(len(extracted_values)-int(device_id)-1)]
    if device["devAddr"][-2]=='B':
        if len(device["temperatureSensor_3"])>0:
            df = pd.DataFrame(device["temperatureSensor_3"][2:])
            df['time']=pd.to_datetime(df['time'])
            df.rename(columns={'value': device["devName"]}, inplace=True)
            if not WM200_realx10_axed:
                WM200_realx10_ax=df.plot(x='time',y=device["devName"],xlabel="Time",ylabel=sensortypes["WM"]["temperatureSensor_3"])
                WM200_realx10_axed=True
            else:
                df.plot(x='time',y=device["devName"],ax=WM200_realx10_ax,xlabel="Time")
fig = plt.gcf()
fig.set_size_inches(12,9)
fig.savefig("WM2_WM_WM200_realx10.png",bbox_inches="tight")
#         # https://pandas.pydata.org/docs/reference/api/pandas.DataFrame.plot.html
#         # In [12]: pd.to_datetime(pd.Series(['05/23/2005']), format="%m/%d/%Y")
#     # plt.show()

