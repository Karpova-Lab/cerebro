import pandas as pd
from pandas import DataFrame, read_csv,concat,to_timedelta,to_datetime,to_numeric
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.legend_handler import HandlerLine2D

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
    setupKeys = ['expDate','rigNum','ratNum','cerebroNum','baseFirmware']
    setup = {}
    for i,setupKey in enumerate(setupKeys): #assign the header data to variables that we can recall later to print
        setup[setupKey] = bLogHeader.loc[0,i]
    #Read in base station log. Skip the experimental setup
    bLog = pd.read_csv(logFile,sep=',',skiprows=1,names=['bTime','Sent','P1','P2','P3','P4','P5'])
    bLog['bTime'] = to_numeric(bLog['bTime'],errors='coerce')   #converts time columns from strings to numbers
    bLog = bLog[bLog['Sent']!='Save Sent']                              #remove save events
    trigsandchanges = len(bLog)
    bLog = bLog[bLog['Sent']!='New Parameters Sent']                    #remove parameter change events
    changesonly = len(bLog)
    numChangesSent = trigsandchanges - changesonly
    bLog = reIndex(bLog)
    bLog.drop(bLog.columns[[2,3,4,5,6]], axis=1, inplace=True)          #remove  parameter columns
    first = (bLog.loc[0,'bTime'])                                   #store time offset
    bLog['bTime'] = bLog['bTime']-first                         #subtract offset from all timestamps
    bLog['bDiff'] = bLog['bTime']-bLog['bTime'].shift(1)        #add column of time between events
    bLog['Clock'] = bLog['bTime'].apply(readableTime)               #add column with readable time
    sessionLength = bLog.loc[len(bLog)-1,'Clock']
    bData = {'numChangesSent':numChangesSent,'setup':setup,'sessionLength':sessionLength}
    return bLog,bData

def parseCerebroLog(logFile):
    # get cerebro attributes from cerebro log
    cLog = pd.read_csv(logFile,names=['cTime','Received'])
    firmware = cLog.loc[0,'Received']                                   #get firmware version
    cLog = cLog[1:-20]                                                  #remove firwmare row and calibration vector at the end
    cLog = reIndex(cLog)
    #label events
    paramChangeIndices = cLog[cLog['cTime'].str.contains("Start Delay")].index.tolist() #get indices of parameter changes
    paramNames = ['delay','onTime','offTime','trainDur','rampdown']
    #get parameters from the parameter rows and then use them to label the events above them that they describe
    for i,paramChangeIndex in enumerate(paramChangeIndices):
        for j,paramName in enumerate(paramNames):
            b = paramChangeIndices[i-1]+len(paramNames) if i>0 else 0 #b is the index of where above the parameters to start labeling events.
            cLog.loc[b:paramChangeIndex-1,paramName] = cLog.loc[paramChangeIndex+j,'Received']
    cLog['cTime'] =to_numeric(cLog['cTime'],errors='coerce')    #convert time column from strings to numbers
    cLog = cLog[cLog['cTime'].notnull()]                            #remove parameter rows from data frame, leaving of list of events w/parameter labels
    cLog = reIndex(cLog)

    first = cLog.loc[0,'cTime']                                     #get time offset
    cLog['cTime'] = cLog['cTime']-first                         #subtract offset from all timestamps
    cLog['cDiff'] = cLog['cTime']-cLog['cTime'].shift(1)        #add column of time between events

    orig = np.array(paramChangeIndices)
    transform = np.array(range(0,5*len(paramChangeIndices),5))
    ends = orig-transform
    starts = np.insert(ends,0,0)[:-1]+1
    paramRanges = ""
    for start,end in zip(starts,ends):
        if start<end:
            for col in range(2,7):
                paramRanges = paramRanges + ("{}\t\t").format(cLog.iloc[start,col])
            paramRanges = paramRanges + ('[{}-{}]\n'.format(start,end))
    cData = {'firmware':firmware,'paramNames':paramNames,'paramChangeIndices':paramChangeIndices,'paramRanges':paramRanges}
    return cLog,cData

def compare(bLog,cLog,paramNames):
    #combine the logs
    baseCols = ['Clock','Sent','bTime','bDiff']
    cerebroCols = ['cDiff','cTime','Received',]+ paramNames
    combined = concat([bLog[baseCols],cLog[cerebroCols]],axis=1,) # concatenate the two dataframes

# **Compare the data logs to reveal any events may have been sent by Base Station, but not executed by Cerebro**

# We are comparing timestamps of events. We expect there to be a fairly constant ratio between
# Base Station timestamps and Cerebro timestamps (it is not 1:1 because both MCUs are using internal clocks
# that are not neccesarily calibrated, and also have some drift).

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
    for j in np.arange(dataLength)[1:-1]: # if there are 8 entries, we want indices [1,2,3,4,5,6,7]
        if numMiss == 0: #We've found all of the missing signals, so the remaining entries match and we can exit
            break
        baseInterval = baseInterval + combined['bDiff'][j] #keep track of how long it's been since we successfully sent a command from base station
        percentDiff = abs(1-combined['bTime'][j]/combined['cTime'][j])*100 #%difference between base station time and cerebro time
        changingDiff = abs(percentDiff-correctDiff)
        alt_percentDiff = abs(1-combined['bTime'][j+1]/combined['cTime'][j])*100
        alt_changingDiff = abs(alt_percentDiff-correctDiff)
        if changingDiff-alt_changingDiff>0.01 and j>3:
            missDiff.append(abs(percentDiff-correctDiff))
            combined[cerebroCols] = combined[cerebroCols][:j].append(combined[cerebroCols][j:].shift(1))
            if combined['Sent'][j] == "Trigger Sent":
                singleDuration = int(combined['onTime'][j+1])+int(combined['rampdown'][j+1])
                trainDuration = int(combined['trainDur'][j+1])
                trueDuration = max(singleDuration,trainDuration) #duration that cerebro will be busy executing a waveform
                if int(baseInterval)<trueDuration:
                    ignoreVec.append(j)
                    for column in paramNames:
                        combined.loc[j,column] = "Ignored"
                else:
                    missedTrigVec.append(j)
                    for column in paramNames:
                        combined.loc[j,column] = "Trig Miss"
            elif combined['Sent'][j] == "Continue Sent":
                missedContinueVec.append(j)
                for column in paramNames:
                        combined.loc[j,column] = "Continue Miss"
            elif combined['Sent'][j] == "Stop Sent":
                missedStopVec.append(j)
                for column in paramNames:
                        combined.loc[j,column] = "Stop Miss"
            numMiss = numMiss-1
        else:
            baseInterval = 0 #reset the time since we've successfully sent a command from base station
            correctDiff = percentDiff #update our standard %diff which we are comparing to
            diffArray.append(correctDiff)
#             baseVec.append(combined['bTime'][j])

    # create dictionary with summary data
    compareData = {'missedStopVec':missedStopVec,'missedTrigVec':missedTrigVec,'missedContinueVec':missedContinueVec,'ignoreVec':ignoreVec}
    compareData['trigIgnored'] = len(ignoreVec)
    compareData['trigMissed'] = len(missedTrigVec)
    compareData['continueMissed'] = len(missedContinueVec)
    compareData['stopMissed'] = len(missedStopVec)
    compareData['trigSent'] = np.sum(combined['Sent']=='Trigger Sent')
    compareData['trigReceived'] = np.sum(combined['Received']=='trigger')
    compareData['continueSent'] = np.sum(combined['Sent']=='Continue Sent')
    compareData['continueReceived'] = np.sum(combined['Received']=='continue')
    compareData['stopSent'] = np.sum(combined['Sent']=='Stop Sent')
    compareData['stopReceived'] = np.sum(combined['Received']=='abort')

#     plt.figure(figsize=(17,11))
#     plt.plot(baseVec,diffArray,'gx',label="Base Station")
#     plt.ylabel("%Diff between timestamps",fontsize = 25)
#     plt.xlabel("Time (ms)",fontsize = 25)

#     print "Percent Diff range: {} to {} ({})".format(round(min(diffArray),2),round(max(diffArray),2),round(max(diffArray)-min(diffArray),3))
#     bob = np.array(diffArray)
#     jay = np.array(diffArray)
#     diff = abs(bob[1:]-jay[:-1])
#     print "max intra diff:\t", round(max(diff),3)
#     print "min error\t",round(min(missDiff),3)
#     print
    return combined,compareData

def printSummary(combined,sessionLength,paramRanges,comp):
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
    # summary['tIgnored'] = '\t{} Ignored: {}'.format(tIgnored,ignoredVec)
    summary['tMissed'] = '\t{} Missed: {}'.format(tMissed,missedTrigVec)
    summary['continueSummary'] = 'Continue Success Rate:\t{}/{} ({:.2f}%)'.format(cReceived,cSent,cReceived/float(cSent)*100)
    summary['abortSummary'] = 'Stop Success Rate:\t{}/{} ({:.2f}%)'.format(sReceived,sSent,sReceived/float(sSent)*100)
    summary['sMissed'] = '\t{} Missed: {}'.format(sMissed,missedStopVec)
    summary['paramRanges'] = '\nParameters Throughout Session:\ndelay\t\ton\t\toff\t\ttrain\t\tramp\t\t[range]\n{}'.format(paramRanges)

    print (summary['lengthSummary'])
    print (summary['triggerSummary'])
    # print (summary['tIgnored'])
    print (summary['tMissed'])
    print (summary['continueSummary'])
    print (summary['abortSummary'])
    print (summary['sMissed'])
    print (summary['paramRanges'])

    summary['tMissed'] = summary['tMissed'].replace("\t",'\t\t')
    summary['sMissed'] = summary['sMissed'].replace("\t",'\t\t')

    return summary

def writeSummary(cerebroLogPath,bData,cData,summary,combined):
    setup = bData['setup']
    target = open('{}summary.txt'.format(cerebroLogPath[:-14]), 'w')
    target.write('Session Start:{}'.format(to_datetime(setup['expDate']).strftime("%Y-%m-%d %H:%M:%S")))
    target.write('\nRat:{} '.format(setup['ratNum']))
    target.write('\nRig:{}'.format(setup['rigNum']))
    target.write('\nCerebro:{:1}'.format(int(setup['cerebroNum'])))
    target.write('\nBase Station Firmware Version:{}'.format(int(setup['baseFirmware'])))
    target.write('\nCerebro Firmware Version:{}'.format(cData['firmware']))
    target.write('\n\n{}'.format(summary['lengthSummary']))
    target.write('\n{}'.format(summary['triggerSummary']))
    # target.write('\n{}'.format(summary['tIgnored']))
    target.write('\n{}'.format(summary['tMissed']))
    target.write('\n{}'.format(summary['continueSummary']))
    target.write('\n{}'.format(summary['abortSummary']))
    target.write('\n{}'.format(summary['sMissed']))
    target.write('\n{}'.format(summary['paramRanges']))
    target.write('\n')
    target.close()
    combined.to_csv('{}combined_log.csv'.format(cerebroLogPath[:-14]),',')  #save csv file
    target = open('{}summary.html'.format(cerebroLogPath[:-14]), 'w')
    target.write('{}'.format(combined.to_html()))
    target.close()
#     bk.save(p2,'{}newgraph.html'.format(cerebroLogPath[:-14]))

def matplotlibGraph(cerebroLogPath,bothDF,compData,sumry):
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
    plt.savefig('{}graph.png'.format(cerebroLogPath[:-14]))
    plt.show()
