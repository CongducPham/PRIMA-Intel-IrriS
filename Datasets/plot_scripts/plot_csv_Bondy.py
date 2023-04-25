import pandas as pd
import matplotlib.pyplot as plt

## Read CSV
# df=pd.read_csv('extracted_Bondy_aggregated.csv', header=0, skiprows=[1,2])  
df=pd.read_csv('extracted_Bondy_aggregated.csv', header=[0,1,2])  

## Manual removal of some columns (column 2 here in e.g.)
# df.drop(labels=df.columns[2],axis=1,inplace=True)

## Use Pandas interpretation of date strings
df[df.columns[0]]=pd.to_datetime(df[df.columns[0]],utc=True)

## Group column IDs according to considered metrics 
same_cols={}
level0=["time"]
cur_dev=df.columns[1][0]
for col in range(1,len(df.columns)-1):
    if "Unnamed:" in df.columns[col][0]:
        level0.append(cur_dev)
    else:
        cur_dev=df.columns[col][0]
        level0.append(cur_dev)

    if df.columns[col][2] in same_cols:
        same_cols[df.columns[col][2]].append(col)
    else:
        same_cols[df.columns[col][2]]=[col]
# df.columns.set_levels(level0,level=0,inplace=True)

# Using matplotlib directly is also possible instead of inside Pandas, eg:
# plt.plot(df.R2.index.to_pydatetime(), df.R2, 'o-')

# Plot loop
for metric in same_cols:
    # Plot metric as a func of time using Pandas plot func 
    df[[df.columns[0]]+[df.columns[col] for col in same_cols[metric]]].plot.line(marker='o',x=df.columns[0],xlabel="Time",ylabel=metric)
    # Add Device name as legend 
    plt.gca().legend([level0[col] for col in same_cols[metric]])

    # Controls saved figure parameters
    fig = plt.gcf()
    fig.set_size_inches(12,9)
    fig.savefig(metric.replace('/','_')+".png",bbox_inches="tight")

# Show plots
# plt.show()