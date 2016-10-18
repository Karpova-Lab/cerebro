from calFunctions import*
from sys import argv
script, calibDataPath,wantedLevel,showGraph = argv

calibDataPath = calibDataPath[1:-1]

sourceName = calibDataPath
wantedLevel = int(wantedLevel)
showGraph = int(showGraph)
# wantedLevel = 8

LD,measuredPowers,saveName = parseMeterFile(sourceName)
peakIndices,peakVals,threshold = getPeaks(measuredPowers)
plt.figure(figsize=(17,11))
rawGraph(measuredPowers,peakIndices,peakVals,LD) #subplot 1
trimmedInputs,lightout = calibrationCurve(peakIndices,peakVals,wantedLevel,LD) #subplot 2

if showGraph>0:
	fig = plt.gcf()
	fig.subplots_adjust(hspace=0.3) #add space between subplots
	plt.show()

getCalVector(wantedLevel,LD,trimmedInputs,lightout)
saveSummary(saveName,trimmedInputs,lightout,LD)


