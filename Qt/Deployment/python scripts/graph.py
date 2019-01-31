
# coding: utf-8

# In[ ]:


isNotebook = False
if isNotebook:
    from rawDataSelector import*
else:
    from sys import argv
    script, basestationLogPath = argv
    
import json
import pandas as pd
import numpy as np
from os import getcwd
import webbrowser
from time import sleep
########matplotlib#############
import matplotlib.pyplot as plt
import matplotlib.style as style 
#######################################  BOKEH   ####################################################
from bokeh.plotting import figure, reset_output,output_notebook,output_file,show,save,ColumnDataSource
from bokeh.models import WheelZoomTool
from bokeh.models.annotations import Title

scriptVersion = 5

def getHeaderInfo(dataFile):
    numSetupVars = 8
    firstCol = pd.read_csv(dataFile,sep=',',header=None,skip_blank_lines=False,names=['Parameter','Value'],usecols=['Parameter','Value'])
    startMask = firstCol['Parameter']=='~~~~~~~~~New Session~~~~~~~~~'
    sessionStart = firstCol[startMask].index[-1]
    dropRows = np.arange(sessionStart)
    header = firstCol[['Parameter','Value']]
    header = header.drop(dropRows)
    header.reset_index(inplace=True)
    header.drop('index',axis=1,inplace=True)
#     print(header.iloc[np.arange(numSetupVars),:])

    logStart = sessionStart+numSetupVars

    batteryStart = header.loc[15,'Value']
    b = pd.to_datetime((header.loc[4,'Value']),format='%Y_%m_%d_%H_%M') #https://pandas-docs.github.io/pandas-docs-travis/generated/pandas.Period.strftime.html
    print(b.strftime('Session started at %I:%M %p on %m/%d/%Y'))
    info = {
        'Xavier_version':header.loc[1,'Value'],
        'Rig':header.loc[2,'Value'],
        'Rat':header.loc[3,'Value'],
        'Start_datetime':header.loc[4,'Value'],
        'Base_version':int(header.loc[5,'Value']),
        'Base_Channel':int(header.loc[6,'Value']),
        'Cerebro_version':int(header.loc[7,'Value']),
        'Cerebro_serial':int(header.loc[8,'Value']),
        'Washer_version':scriptVersion,
    }
    return info,batteryStart,logStart

def createRawLogDF(dataFile,_logStart):
    logEntries = pd.read_csv(dataFile,sep=',',
                         names=['Timestamp','MsgCount','Msg','Val'],
                         skiprows=_logStart,
                        )
    logEntries.loc[:,'Timestamp'] = pd.to_timedelta(logEntries.loc[:,'Timestamp'],errors='coerce')*10**6
    eventsMask = pd.isnull(logEntries['Timestamp'])
    logEntries = logEntries[eventsMask==False]    
    
    missedTable = pd.read_csv(dataFile,sep=',',
                         names=['Description','Value'],
                         skiprows=_logStart,
                        )
    missedSummaryMask = missedTable['Description']=='Missed Message Index'
    totalMask = missedTable['Description']=='Total Missed'
    
    totalMissed = int(missedTable[totalMask][['Description','Value']]['Value'])
    missedSummary = missedTable[missedSummaryMask][['Description','Value']]
    
    if len(missedSummary) != totalMissed:
        print(len(missedSummary),"!=",totalMissed)
        print("WARNING: Missed Length isn't the same as total missed")
    
    return logEntries,missedSummary

def extractFeebackDF(logEntries):
    feedbacks = logEntries.copy()
    feedbackMask = feedbacks['Msg']=='Feedback'
    fbacks = feedbacks[feedbackMask]
    a = fbacks.Val.str.split('-',n=5,expand=True)
    a.columns = ['L_set','L_sense','L_DAC','R_set','R_sense','R_DAC']
    fbacks = fbacks.join(a)
    fbacks = fbacks[['Timestamp','MsgCount','L_set','L_sense','L_DAC','R_set','R_sense','R_DAC']]
    return fbacks

def extractBatteryDF(logEntries):
    batteryFrame = logEntries.copy()
    batteryMask = batteryFrame['Msg']=='Battery'
    batteryFrame = batteryFrame[batteryMask]
    batteryFrame['Val'] = pd.to_numeric(batteryFrame['Val'])
    batteryFrame = batteryFrame[['Timestamp','Val']]
    return  batteryFrame

def extractSentDF(logEntries):
    missedFrame = logEntries.copy()
    feedbackMask = missedFrame['Msg']=='Feedback'
    batteryMask = missedFrame['Msg']=='Battery'
    waveformMask = missedFrame['Msg']=='Waveform'
    powerMask = missedFrame['Msg']=='Diode Powers'
    sent = missedFrame[(batteryMask==False)&(feedbackMask==False)&(waveformMask==False)&(powerMask==False)]
    return sent

def extractWaveDF(logEntries):
    waveFrame = logEntries.copy()
    waveMask = waveFrame['Msg']=='Waveform'
    waveFrame = waveFrame[waveMask]
    a = waveFrame.Val.str.split('-',n=5,expand=True)
    a.columns = ['Start_delay','On_time','Off_time','Train_duration','Ramp']
    waveFrame = waveFrame.join(a)
    waveFrame = waveFrame[['Timestamp','MsgCount','Start_delay','On_time','Off_time','Train_duration','Ramp']]
    return waveFrame

def extractPowerDF(logEntries):
    powerFrame = logEntries.copy()
    waveMask = powerFrame['Msg']=='Diode Powers'
    powerFrame = powerFrame[waveMask]
    a = powerFrame.Val.str.split('-',n=2,expand=True)
    a.columns = ['Left_set','Right_set']
    powerFrame = powerFrame.join(a)
    powerFrame = powerFrame[['Timestamp','MsgCount','Left_set','Right_set']]
    return powerFrame


#####################################################################################

if isNotebook:
    rawDirectory = getcwd()
    myBrowser = fileBrowser_single(
        rawDirectory,
        filterExtension='.csv'
    )

    myBrowser.setupCallbacks()
    myBrowser.showBrowser()


# In[ ]:


if isNotebook:
    def run_all(ev):
        display(Javascript('IPython.notebook.execute_cells_below()'))
        print(myBrowser.finalPath)

    button = widgets.Button(description="Analyze Data")
    button.on_click(run_all)
    display(button)
    ##############################################################################################


# In[ ]:


if isNotebook:
    csvFile = myBrowser.finalPath
else:
    csvFile = basestationLogPath ## if testing from terminal

infoDict,batteryStart,logStart = getHeaderInfo(csvFile)
logDF,missedDF= createRawLogDF(csvFile,logStart)
batteryDF = extractBatteryDF(logDF)
feedbackDF = extractFeebackDF(logDF)
sentDF = extractSentDF(logDF)
waveDF = extractWaveDF(logDF)
powerDF = extractPowerDF(logDF)

combinedDict = {'info':infoDict,
                'rawDF':logDF.to_json(),
                'feedbackDF':feedbackDF.to_json(),
                'batteryDF':batteryDF.to_json(),
                'sentDF':sentDF.to_json(),                
                'missedDF':missedDF.to_json(),
                'waveDF':waveDF.to_json(),
                'powerDF':powerDF.to_json(),                
               }

jsonSaveName = 'temp/clean.json'
with open(jsonSaveName,'w') as saveJSON:
    json.dump(combinedDict,saveJSON,indent=2,sort_keys=True)


# In[ ]:


def dataframeFromJSON(summaryDict,jsonDictName,colNames):
    fromJson = pd.read_json(summaryDict[jsonDictName]).sort_index()[colNames]
    fromJson.loc[:,'Timestamp'] = pd.to_timedelta(fromJson.loc[:,'Timestamp'],errors='coerce')*10**6
    return fromJson.reset_index()

summary  = json.load(open(jsonSaveName))
info = summary['info']
missedDF = pd.read_json(summary['missedDF'])
battDF = dataframeFromJSON(summary,'batteryDF',['Timestamp','Val'])
sentDF = dataframeFromJSON(summary,'sentDF',['Timestamp','MsgCount','Msg','Val'])
rawDF = dataframeFromJSON(summary,'rawDF',['Timestamp','MsgCount','Msg','Val'])
feedbackDF = dataframeFromJSON(summary,'feedbackDF',
                               ['Timestamp','MsgCount','L_set','L_sense','L_DAC','R_set','R_sense','R_DAC'])
waveDF = dataframeFromJSON(summary,'waveDF',['Timestamp','Start_delay','On_time','Off_time','Train_duration','Ramp'])

for k,v in info.items():
    print(k,":",v)


# In[ ]:


reset_output()
output_notebook()

def getDataSets(msgType):
    typeMask = sentDF['Msg']== msgType
    typeDF = sentDF[typeMask]

    missedMessages = missedDF['Value'].values
    if len(missedMessages): #if there are misse messages
        for i,missing in enumerate(missedMessages):
            if i==0:
                missed_msg_mask = typeDF['MsgCount'].astype(int)==missing
            else:
                missed_msg_mask = missed_msg_mask | (typeDF['MsgCount'].astype(int)==missing)
        receivedMsgs = typeDF[missed_msg_mask==False]
        missedMsgs = typeDF[missed_msg_mask==True]
        return receivedMsgs,missedMsgs
    else:
        return typeDF,[]

def create_ColumnDataSource(DF,name):
    try:
        xvals = DF['Timestamp'].dt.seconds.values + DF['Timestamp'].dt.microseconds.values/1000000
        yvals = np.array([name]*len(xvals))
        msgVals = DF['MsgCount'].values

        dataSource = ColumnDataSource(
            data=dict(
                time = xvals*1000,
                milli_time = xvals,
                y = yvals,
                msg = msgVals
            )
        )    
        return dataSource
    except: #sometimes there aren't bad data to create a columndatasource from
        return []
    
#plot setup    
TOOLTIPS = [
    ("Seconds", "@milli_time{0.000}"),
    ("Message", "@msg"),
]

p = figure(
    plot_width=1600,
    plot_height=250,
    y_range=['trigger','continue','abort','feedback'],
    tools="xpan,xwheel_pan,reset",
    tooltips=TOOLTIPS,
    x_axis_type="datetime",
    toolbar_location='left'
)
p.xaxis.axis_label = "Time"
p.yaxis.axis_label = "Message Type"
p.title.align = "center"


p.add_tools(WheelZoomTool(dimensions="width"))
p.toolbar.active_scroll=  p.select_one(WheelZoomTool) 

#data cleaning
trig_good,trig_miss = getDataSets('Trigger')      
goodTrigSource = create_ColumnDataSource(trig_good,'trigger')
badTrigSource = create_ColumnDataSource(trig_miss,'trigger')

continue_good,continue_miss = getDataSets('Continue')      
goodContinueSource = create_ColumnDataSource(continue_good,'continue')
badContinueSource = create_ColumnDataSource(continue_miss,'continue')

filtered_good,filtered_miis = getDataSets('Spam Filtered Abort')      
filtered_source = create_ColumnDataSource(filtered_good,'abort')
filtered_miss_source = create_ColumnDataSource(filtered_miis,'abort')

abort_good,abort_miss = getDataSets('Abort')      
goodAbortSource = create_ColumnDataSource(abort_good,'abort')
badAbortSource = create_ColumnDataSource(abort_miss,'abort')

####################################################
#feedback
suspiciousMask = (abs(feedbackDF['L_set']-feedbackDF['L_sense'])>20) | (abs(feedbackDF['R_set']-feedbackDF['R_sense'])>20)
good_feedbackDF = feedbackDF[suspiciousMask==False]
bad_feedbackDF = feedbackDF[suspiciousMask]

good_feedsource = create_ColumnDataSource(good_feedbackDF,'feedback')
bad_feedsource = create_ColumnDataSource(bad_feedbackDF,'feedback')
#############################################
markerSize = 30

#trigger
p.triangle('time', 'y', size=markerSize, source=goodTrigSource,color='green',fill_color='white',fill_alpha=0)
if badTrigSource:
    p.triangle('time', 'y', size=markerSize, source=badTrigSource,color='green',fill_color='red')

#continue
p.diamond('time', 'y', size=markerSize, source=goodContinueSource,color='green',fill_color='white',fill_alpha=0)
if badContinueSource:
    p.diamond('time', 'y', size=markerSize, source=badContinueSource,color='green',fill_color='red')

#abort
p.square_x('time', 'y', size=markerSize, source=filtered_source,color='green',fill_color='yellow',alpha=0.3)
if filtered_miss_source:
    p.square_x('time', 'y', size=markerSize, source=filtered_miss_source,color='green',fill_color='red',alpha=0.3)
p.square('time', 'y', size=markerSize, source=goodAbortSource,color='green',fill_color='white',fill_alpha=0)
if badAbortSource:
    p.square('time', 'y', size=markerSize, source=badAbortSource,color='green',fill_color='red')

#feeback
p.circle('time', 'y', size=markerSize, source=good_feedsource,fill_color='white',fill_alpha=0)
if bad_feedsource:
    p.circle('time', 'y', size=markerSize, source=bad_feedsource,fill_color='orange',fill_alpha=0.3)

t = Title()
t.text = "Missed Messages: {}     Bad Feedback: {}".format(
    len(missedDF),
    suspiciousMask.sum()
)
t.align = 'center'
p.title = t
show(p)


# In[ ]:


# plotBattery(battDF)
# plotFeedback(feedbackDF)

times = np.array(feedbackDF['Timestamp'],dtype=np.uint64)/10**9/3600

lSet = np.array(feedbackDF['L_set'],dtype=int)
lSense = np.array(feedbackDF['L_sense'],dtype=int)

rSet = np.array(feedbackDF['R_set'],dtype=int)
rSense = np.array(feedbackDF['R_sense'],dtype=int)

yMax = max(lSet.max(),rSet.max())*1.1

fig, ax1 = plt.subplots(figsize=(11,7),dpi=100)
ax1.scatter(times,lSense,label = 'Left Feeback Readings')
ax1.plot(times,lSet,label='Left Set Point',linewidth=5,alpha = 0.5)
ax1.scatter(times,rSense,label = 'Right Feedback Readings')
ax1.plot(times,rSet,label='Right Set Point',linewidth=5,alpha = 0.5)

ax1.set_ylabel('Readings from phototransistor feedback')
ax1.set_ylim(-.05*yMax,yMax*1.05)
ax1.set_xlabel('Time (hours)')

# handles, labels = ax1.get_legend_handles_labels()
# ax1.legend(handles, labels,ncol=2,loc='lower center')
ax1.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)

###### Battery Plot

batt_times = np.array(battDF['Timestamp'],dtype=np.uint64)/10**9/3600
vals = np.array(battDF['Val'],dtype=int)

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
ax2.plot(batt_times,vals,'-o',c='C2',linewidth=2)
ax2.tick_params(axis='y', colors='C2')
ax2.set_ylabel('Battery %',color='C2',rotation=270)
ax2.set_ylim(-5,100)
ax2.grid(linestyle='--',axis='y',color='C2',alpha=0.50)

bbox_props = dict(boxstyle="round", fc="white")
battery_summary = 'Discharge Rate:\n{:.1f}% per hour'.format((100-vals[-1])/batt_times[-1])
# ax2.annotate(battery_summary,xy=(batt_times[-1]*.96,vals[-1]*.85),bbox=bbox_props,ha='center',va='center',color='green',alpha=.1)  
ax2.text(s=battery_summary,x=1.15,y=.5,
         transform=ax2.transAxes,
         bbox=bbox_props,ha='center',va='center',color='green')  


ax2.set_title("Animal: {}     Session: {}".format(info['Rat'],info['Start_datetime']))

matlabSaveName = 'temp/summary.png'
plt.savefig(matlabSaveName,bbox_inches = "tight")
# plt.show()


# In[ ]:


#https://stackoverflow.com/questions/50807744/apply-css-class-to-pandas-dataframe-using-to-html
pd.set_option('colheader_justify', 'center')   # FOR TABLE <th>

html_string = '''
<html>
  <head><title>Feedback Dataframe</title></head>
  <link rel="stylesheet" type="text/css" href="df_style.css"/>
  <body>
    {table}
  </body>
</html>.
'''

# OUTPUT AN HTML FILE
saveName = 'temp/feedback.html'
with open(saveName, 'w') as f:
    f.write(html_string.format(table=feedbackDF.to_html(classes='mystyle')))
webbrowser.open('file:///{}/{}'.format(getcwd(),saveName))
sleep(1)


# In[ ]:


#https://stackoverflow.com/questions/50807744/apply-css-class-to-pandas-dataframe-using-to-html
pd.set_option('colheader_justify', 'center')   # FOR TABLE <th>

html_string = '''
<html>
  <head><title>Raw Dataframe</title></head>
  <link rel="stylesheet" type="text/css" href="df_style.css"/>
  <body>
    {table}
  </body>
</html>.
'''

# OUTPUT AN HTML FILE
saveName = 'temp/raw.html'
with open(saveName, 'w') as f:
    f.write(html_string.format(table=rawDF.to_html(classes='mystyle')))
webbrowser.open('file:///{}/{}'.format(getcwd(),saveName))
sleep(1)


# In[ ]:


reset_output()
bokehGraphName = 'temp/Summary.html'
output_file(bokehGraphName)
html_file_location = save(p)

with open(bokehGraphName, 'r+') as fd:
    contents = fd.readlines()
    for i,j in enumerate(contents):
        if (j.find('<body>')>0):
            bodyStarts = i+1

    contents.insert(bodyStarts, '<center><img src="{}"></center>'.format('summary.png'))  # new_string should end in a newline
    fd.seek(0)  # readlines consumes the iterator, so we need to start over
    fd.writelines(contents)  # No need to truncate as we are increasing filesize
    
import webbrowser
webbrowser.open('file:///{}'.format(html_file_location))

