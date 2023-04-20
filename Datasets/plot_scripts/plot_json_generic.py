import json
import pandas as pd
import matplotlib.pyplot as plt

with open('full_JSON_export.json') as f:
   extracted_values = json.load(f)

# collect sensor types
sensortypes=[]
for device_id in extracted_values:
    device=extracted_values[device_id]
    for existing_sensor in device["sensors"]:
        if not (existing_sensor["pretty_name"] in sensortypes):
            sensortypes.append(existing_sensor["pretty_name"])


for sens_type in sensortypes:
    # Temperature plot 
    fig_axed=False

    for device_id in extracted_values:
        device=extracted_values[device_id]
        
        found=False
        for existing_sensor in device["sensors"]:
            if existing_sensor["pretty_name"]==sens_type:
                found=True
                break
        if found and len(existing_sensor["values"])>0:
            df = pd.DataFrame(existing_sensor["values"][2 if existing_sensor["id"][-1] in ["0","1","2","3"] else 1:])
            df['time']=pd.to_datetime(df['time'])
            df.rename(columns={'value': device["devName"]}, inplace=True)
            if not fig_axed:
                fig_ax=df.plot(x='time',y=device["devName"],xlabel="Time",ylabel=sens_type)
                fig_axed=True
            else:
                df.plot(x='time',y=device["devName"],ax=fig_ax,xlabel="Time")

    fig = plt.gcf()
    fig.set_size_inches(12,9)
    fig.savefig(sens_type.replace('/','_').replace(' ','_')+".png",bbox_inches="tight")

#         # https://pandas.pydata.org/docs/reference/api/pandas.DataFrame.plot.html
#         # In [12]: pd.to_datetime(pd.Series(['05/23/2005']), format="%m/%d/%Y")
#     # plt.show()

