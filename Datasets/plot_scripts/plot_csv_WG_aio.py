import pandas as pd
import matplotlib.pyplot as plt


INTELIRRIS=True
# INTELIRRIS=False

df=pd.read_csv('extracted_data.csv', header=[0,1])#.dropna()

## Group column IDs according to considered metrics 
same_cols={}
devnames={}
sensorids={}
for col in range(len(df.columns)):
    # convert times with pandas and get device names
    if col%2==0:
        df[df.columns[col]]=pd.to_datetime(df[df.columns[col]],utc=True)
        devnames[col+1]=df.columns[col][1]
        if len(devnames[col+1])>2:
            # check duplicates
            if devnames[col+1][-2]==".":
                devnames[col+1]=devnames[col+1][:-2]
            elif devnames[col+1][-3]==".":
                devnames[col+1]=devnames[col+1][:-3]
    # get which devices are involved in each metric
    if col%2==1:
        # check duplicates
        colname=df.columns[col][1]
        sensid=df.columns[col][0]
        sensorids[col]=sensid
        if colname[-2]==".":
            colname=colname[:-2]
        if colname[-3]==".":
            colname=colname[:-3]
        if colname in same_cols:
            same_cols[colname].append(col)
        else:
            same_cols[colname]=[col]
# df.columns.set_levels(level0,level=0,inplace=True)
# print(same_cols)
# print(sensorids)

# Plot loop
for metric in same_cols:
    # Plot metric as a func of time using Matplotlib plot func 
    for col in same_cols[metric]:
        start_index=0
        if INTELIRRIS:
            start_index=2 if sensorids[col][-1] in ["0","1","2","3"] else 1
        col_len=len(df[df.columns[col]].dropna())
        plt.plot(
            [df[df.columns[col-1]][i].to_pydatetime() for i in range(start_index,col_len)],
            # df[df.columns[col]].dropna(),
            [df[df.columns[col]][i] for i in range(start_index,col_len)],
            )
    plt.gca().legend([devnames[col] for col in same_cols[metric]])
    plt.gca().set_xlabel("Time")
    plt.gca().set_ylabel(metric)

    fig = plt.gcf()
    fig.set_size_inches(12,9)
    fig.savefig(metric.replace('/','_')+".png",bbox_inches="tight")
    fig.clear()

# # Individual plots + show()
# for col in range(len(df.columns)):
#     if col%2==0:
#         my_met=""
#         for metric in same_cols:
#             if col+1 in same_cols[metric]:
#                 my_met=metric
#                 break
#         df[[df.columns[col]]+[df.columns[col+1]]].plot.line(marker='o',x=df.columns[col],xlabel="Time",ylabel=my_met)
#         plt.gca().legend([devnames[col+1]])

#         fig = plt.gcf()
#         fig.set_size_inches(12,9)
#         fig.savefig(df.columns[col+1][1].replace('/','_')+".png",bbox_inches="tight")
#         plt.show()    
#         fig.clear()

