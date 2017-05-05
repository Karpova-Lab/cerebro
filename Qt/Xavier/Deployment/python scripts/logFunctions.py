import pandas as pd
from pandas import DataFrame, read_csv,concat,to_timedelta,to_datetime,to_numeric
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.legend_handler import HandlerLine2D
import json

def reIndex(dataframe):
    dataframe.reset_index(inplace=True)
    dataframe.drop('index',axis=1,inplace=True)
    return dataframe

#function for converting ms timestamp into readable time (hours, minutes, seconds)
def readableTime(stamp):
    vec = []
    for i in [1000*60*60, 1000*60, 1000.0]:
        vec.append(stamp/i)
        stamp = stamp%i
    return '{}h {}m {}s'.format(int(float(vec[0])),int(float(vec[1])),int(float(vec[2])))


def parseBaseStation(logFile):
    # get experimental setup data from the base station log's first row
    bLogHeader = pd.read_csv(logFile,header=None,nrows=1)
    firstLineKeys = ['startTime','rigNum','ratNum','cerebroNum','baseFirmware','filterDuration']
    bData = {}
    for i,setupKey in enumerate(firstLineKeys): #assign the header data to variables that we can recall later to print
        bData[setupKey] = bLogHeader.loc[0,i]
    #Read in base station log. Skip the experimental setup
    bLog = pd.read_csv(logFile,sep=',',skiprows=1,names=['bTime','Sent','P1','P2','P3','P4','P5'])
    bLog['bTime'] = to_numeric(bLog['bTime'],errors='coerce')   #converts time columns from strings to numbers
    bLog = bLog[bLog['Sent']!='Save Sent']                              #remove save events
    trigsandchanges = len(bLog)
    bLog = bLog[bLog['Sent']!='New Parameters Sent']                    #remove parameter change events
    changesonly = len(bLog)
    bData['numChangesSent'] = trigsandchanges - changesonly
    bLog = reIndex(bLog)
    bLog.drop(bLog.columns[[2,3,4,5,6]], axis=1, inplace=True)          #remove  parameter columns
    first = (bLog.loc[0,'bTime'])                                   #store time offset
    bLog['bTime'] = bLog['bTime']-first                         #subtract offset from all timestamps
    bLog['bDiff'] = bLog['bTime'].diff()                            #add column of time between events
    bLog['Clock'] = bLog['bTime'].apply(readableTime)               #add column with readable time
    bData['sessionLength'] = bLog.loc[len(bLog)-1,'Clock']
    return bLog,bData

def parseCerebroLog(logFile):
    # get cerebro attributes from cerebro log
    cLog = pd.read_csv(logFile,names=['cTime','Received','Light','extra2'])

    # get firmware and hardware info form first 3 lines
    cData = {'firmware':cLog.loc[0,'Received']}
    cData['cerebroNum'] = cLog.loc[1,'Received']
    cData['laserdiodeNum'] = cLog.loc[2,'Received']

    cData['paramNames'],cData['waveform'] = getWaveformParamters(cLog[-25:-20]) # get list of soon to be column headings and create a dictionary with the waveforms parameters
    cData['waveform']['rampVector'] = getRampVector(cLog[-20:]) # add rampvector definition to waveform dictionary

    cLog = cLog[3:-20]    #remove the hardware and firmware info as well as the ramp vector.

    #label events with the correct pulse parameters.
    paramChangeIndices = cLog[cLog['cTime'].str.contains("Start_Delay")].index.tolist() #get indices of parameter changes
    #get parameters from the parameter rows and then use them to label the events above them that they describe
    for i,paramChangeIndex in enumerate(paramChangeIndices):
        for j,paramName in enumerate(cData['paramNames']):
            b = paramChangeIndices[i-1]+len(cData['paramNames']) if i>0 else 0 #b is the index of where above the parameters to start labeling events.
            cLog.loc[b:paramChangeIndex-1,paramName] = cLog.loc[paramChangeIndex+j,'Received']

    cLog['cTime'] =to_numeric(cLog['cTime'],errors='coerce')    #convert time column from strings to numbers
    cLog = cLog[cLog['cTime'].notnull()]                        #remove parameter rows from data frame, leaving of list of events w/parameter labels
    cLog = reIndex(cLog)

    # Add cDiff column
    first = cLog.loc[0,'cTime']                                 #get time offset
    cLog['cTime'] = cLog['cTime']-first                         #subtract offset from all timestamps
    cLog['cDiff'] = cLog['cTime'].diff()                        #add column of time between events

    cLog = cLog.drop(['extra2'], 1) #remove extra columns
    return cLog,cData

def getWaveformParamters(_wavformDF):
    paramNames = list(_wavformDF.cTime.values)
    paramValues = list(map(int,_wavformDF.Received.values))
    return paramNames[1:],dict(zip(paramNames,paramValues))

def getRampVector(_rampDF):
    rampVec = []
    for row in _rampDF.itertuples():
        for column in [1,2,3,4]:
            rampVec.append(int(row[column]))
    return rampVec


def compare(bLog,cLog,cerColHeaders):
    #combine the logs
    baseCols = ['Clock','Sent','bTime','bDiff']
    cerebroCols = ['cDiff','cTime','Received'] + cerColHeaders + ['Light']
    combined = concat([bLog[baseCols],cLog[cerebroCols]],axis=1,) # concatenate the two dataframes

# **Compare the data logs to reveal any events may have been sent by Base Station, but not executed by Cerebro**

# We are comparing timestamps of events. We expect there to be a fairly constant ratio between
# Base Station timestamps and Cerebro timestamps (it is not 1:1 because both MCUs are using internal clocks
# that are not neccesarily calibrated, and also have some drift).
    Debug = False
    missedStopVec = []
    missedTrigVec = []
    missedContinueVec = []
    ignoreVec = []
    baseInterval = 0
    diffArray = []
    missDiff = []
    correctDiff = abs(1-combined['bTime'][1]/combined['cTime'][1])*100
    dataLength = np.isfinite(combined['bTime']).sum()
    numMiss =  dataLength-np.isfinite(cLog['cTime']).sum() #discrepency between the number of base station entries and cerebro entries
    print("percentDiff\t","changingDiff\t","alt_percentDiff\t","alt_changingDiff\t") if Debug==1 else ""
    for j in np.arange(dataLength)[1:-1]: # if there are 8 entries, we want indices [1,2,3,4,5,6,7]
        if numMiss == 0: #We've found all of the missing signals, so the remaining entries match and we can exit
            break
        baseInterval = baseInterval + combined['bDiff'][j] #keep track of how long it's been since we successfully sent a command from base station
        percentDiff = abs(1-combined['bTime'][j]/combined['cTime'][j])*100 #%difference between base station time and cerebro time for the current row
        alt_percentDiff = abs(1-combined['bTime'][j+1]/combined['cTime'][j])*100

        changingDiff = abs(percentDiff-correctDiff)
        alt_changingDiff = abs(alt_percentDiff-correctDiff)

        print('{} {:.2f}\t\t\t{:.2f}\t\t{:.2f}\t\t\t{:.2f}\t\t{:2f}\t{:2f}'.format(j,percentDiff,changingDiff,alt_percentDiff,alt_changingDiff,changingDiff-alt_changingDiff,percentDiff-alt_percentDiff)) if Debug==1 else ""
        # if the % diff of the alternative is smaller than the current %diff and closer to the previous % diff, then
        # the current cerebro time matches the next base station time better than it matches with the current base staion time,
        # implying that we're missing the cerebro entry that should be matched with the current base station entry
        messagesMatch = ['Trigger Sent', 'Continue Sent', 'Stop Sent'].index(combined['Sent'][j]) == ['trigger','continue','abort'].index(combined['Received'][j]) #check if sent and recieved messages match
        if (alt_percentDiff<percentDiff  and alt_changingDiff<changingDiff) or not(messagesMatch):
            print("missing event") if Debug==1 else ""
            missDiff.append(abs(percentDiff-correctDiff))
            combined[cerebroCols] = combined[cerebroCols][:j].append(combined[cerebroCols][j:].shift(1))
            if combined['Sent'][j] == "Trigger Sent":
                singleDuration = int(combined[cerColHeaders[1]][j+1])+int(combined[cerColHeaders[4]][j+1])
                trainDuration = int(combined[cerColHeaders[3]][j+1])
                trueDuration = max(singleDuration,trainDuration) #duration that cerebro will be busy executing a waveform
                if int(baseInterval)<trueDuration:
                    ignoreVec.append(j)
                    for column in cerColHeaders:
                        combined.loc[j,column] = "Ignored"
                else:
                    missedTrigVec.append(j)
                    for column in cerColHeaders:
                        combined.loc[j,column] = "Trig Miss"
            elif combined['Sent'][j] == "Continue Sent":
                missedContinueVec.append(j)
                for column in cerColHeaders:
                        combined.loc[j,column] = "Continue Miss"
            elif combined['Sent'][j] == "Stop Sent":
                missedStopVec.append(j)
                for column in cerColHeaders:
                        combined.loc[j,column] = "Stop Miss"
            numMiss = numMiss-1
        else:
            baseInterval = 0 #reset the time since we've successfully sent a command from base station
            correctDiff = percentDiff #update our standard %diff which we are comparing to
            diffArray.append(correctDiff)

    # create dictionary with summary data
    compareData = {'missedStopVec':list(map(int,missedStopVec)),'missedTrigVec':list(map(int,missedTrigVec)),'missedContinueVec':list(map(int,missedContinueVec)),'ignoreVec':list(map(int,ignoreVec))}
    missedStopVec =  list(map(int,missedStopVec))
    compareData['trigIgnored'] = int(len(ignoreVec))
    compareData['trigMissed'] = int(len(missedTrigVec))
    compareData['continueMissed'] = int(len(missedContinueVec))
    compareData['stopMissed'] = int(len(missedStopVec))

    compareData['trigSent'] = int(np.sum(combined['Sent']=='Trigger Sent'))
    compareData['trigReceived'] = int(np.sum(combined['Received']=='trigger'))
    compareData['continueSent'] = int(np.sum(combined['Sent']=='Continue Sent'))
    compareData['continueReceived'] = int(np.sum(combined['Received']=='continue'))
    compareData['stopSent'] = int(np.sum(combined['Sent']=='Stop Sent'))
    compareData['stopReceived'] = int(np.sum(combined['Received']=='abort'))

    return combined,compareData

def printSummary(combined,sessionLength,cerebro,comp):
    tSent,tReceived = comp['trigSent'],comp['trigReceived']
    tMissed,missedTrigVec = comp['trigMissed'],comp['missedTrigVec']
    tIgnored,ignoredVec = comp['trigIgnored'],comp['ignoreVec']

    cSent,cReceived = comp['continueSent'],comp['continueReceived']
    cMissed,missedContinueVec = comp['continueMissed'],comp['missedContinueVec']

    sSent,sReceived = comp['stopSent'],comp['stopReceived']
    sMissed,missedStopVec =  comp['stopMissed'], comp['missedStopVec']

    summary = {}
    summary['lengthSummary'] = 'Session Length:\t{}'.format(sessionLength)
    summary['triggerSummary'] = '\nTrigger Success Rate:\t{}/{} ({:.2f}%)'.format(tReceived,tSent-tIgnored,tReceived/float(tSent-tIgnored)*100)
    summary['tMissed'] = '\t{} Missed: {}'.format(tMissed,missedTrigVec)
    summary['continueSummary'] = 'Continue Success Rate:\t{}/{} ({:.2f}%)'.format(cReceived,cSent,cReceived/float(cSent)*100 if cSent>0 else 0)
    summary['abortSummary'] = 'Stop Success Rate:\t{}/{} ({:.2f}%)'.format(sReceived,sSent,sReceived/float(sSent)*100 if sSent>0 else 0)
    summary['sMissed'] = '\t{} Missed: {}'.format(sMissed,missedStopVec)

    print (summary['lengthSummary']+"\n")
    for parameter in cerebro['paramNames']:
        print('{}:\t{} ms'.format(parameter,cerebro['waveform'][parameter]))
    print (summary['triggerSummary'])
    print (summary['continueSummary'])
    print (summary['abortSummary'])

    summary['tMissed'] = summary['tMissed'].replace("\t",'\t\t')
    summary['sMissed'] = summary['sMissed'].replace("\t",'\t\t')

    return summary

def writeSummary(cerebroLogPath,combined,baseData,cerebroData,compareData):
    with open('{}summary.html'.format(cerebroLogPath[:-14]), 'w') as htmlSummary:
        htmlSummary.write('{}'.format(combined.to_html()))

    dataOutput = {
        'Session_Start': baseData['startTime'],
        'Cerebro': [int(baseData['cerebroNum']) , int(cerebroData['cerebroNum'])],
        'Laser_diode': int(cerebroData['laserdiodeNum']),
        'Rig': float(round(baseData['rigNum'],2)),
        'Base_Station_Firmware': int(baseData['baseFirmware']),
        'Cerebro_Firmware': int(cerebroData['firmware']),
        'Base_Station_Filter': int(baseData['filterDuration']),
        'Waveform': cerebroData['waveform'],
        'Analysis':{
            'Session_Length':baseData['sessionLength'],
            'Triggers':{
                'Sent': compareData['trigSent'],
                'Received':compareData['trigReceived'],
                'Missed_Indices':compareData['missedTrigVec'],
                'Ignored_Indices': compareData['ignoreVec']
            },
            'Continues':{
                'Sent':compareData['continueSent'],
                'Received':compareData['continueReceived'],
                'Missed_Indices':compareData['missedContinueVec']
            },
            'Stops':{
                'Sent':compareData['stopSent'],
                'Received': compareData['stopReceived'],
                'Missed_Indices':compareData['missedStopVec']
            }
        }
    }
    with open(cerebroLogPath[:-14]+'summary.json','w') as saveJSON:
        json.dump(dataOutput,saveJSON,indent=2,sort_keys=True)

def matplotlibGraph(cerebroLogPath,bothDF,compData,sumry):
    ###---Alignment Graph---###
    plotTitle = "Time Between\nLogged Events"
    yAxis = "Time since last event (minutes)"
    xAxis = "Event #"

    missedStop = pd.DataFrame({'missIndex':compData['missedStopVec']})
    missedStop['type'] = "Missed Stop"
    missedTrig = pd.DataFrame({'missIndex':compData['missedTrigVec']})
    missedTrig['type'] = "Missed Trig"
    missedContinue = pd.DataFrame({'missIndex':compData['missedContinueVec']})
    missedContinue['type'] = "Missed Continue"
    new = missedStop.append(missedTrig).append(missedContinue)
    new.sort_values(by='missIndex',inplace=True)
    reIndex(new)


    #plot points
    plt.figure(figsize=(17,11))
    bPlot, = plt.plot(range(len(bothDF)),bothDF['bDiff']/60000,'g',label="Base Station")
    cPlot, = plt.plot(range(len(bothDF)),bothDF['cDiff']/60000,'b',label="Cerebro",alpha = 0.5)

    #Display indices of missed signals
    missString = ""
    for i in range(len(new)):
        miss = int(new.loc[i,'missIndex'])
        missType = new.loc[i,'type']
        plt.axvspan(miss-1, miss+1, color='red', alpha=0.5)
        missString+=("{} @ {}\n".format(missType, miss))
    first_legend = plt.legend(handles=[bPlot, cPlot], loc=1,fontsize=20)
    plt.annotate(missString,xy=(0.005,.99),xycoords='axes fraction',size=12,verticalalignment = 'top')

    #Display session summary at the top
    totalSummary = '{}{}\n{}\n{}'.format(sumry['lengthSummary'],sumry['triggerSummary'],sumry['continueSummary'],sumry['abortSummary'])
    totalSummary = totalSummary.replace("\t\t", "\t")
    totalSummary = totalSummary.replace("\t", "      ")
    plt.figtext(.125,.92,totalSummary,fontsize = 12,verticalalignment = 'bottom')

    plt.title(plotTitle,fontsize = 30)
    plt.ylabel(yAxis,fontsize = 25)
    plt.xlabel(xAxis,fontsize = 25)
    plt.savefig('{}alignment.png'.format(cerebroLogPath[:-14]))

    ###---Light Level Graph---###
    plotTitle = "Light Level Distribution"
    yAxis = "Fequency"
    xAxis = "Light Level"
    #plot points
    plt.figure(figsize=(17,11))
    plt.hist(bothDF['Light'][bothDF['Light']>0],bins=list(range(0,1030,10)))
    plt.title(plotTitle,fontsize = 30)
    plt.ylabel(yAxis,fontsize = 25)
    plt.xlabel(xAxis,fontsize = 25)
    # lightPlot, = plt.plot(range(len(bothDF)),bothDF['Light'],c='r',label="Light Level")
    # lightLegend = plt.legend(handles=[lightPlot], loc=1,fontsize=20)

    plt.show()
