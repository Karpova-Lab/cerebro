from calFunctions import*
from sys import argv
script, calDataPath,wantedLevel,showGraph = argv

sourceName = calDataPath[1:-1]
wantedLevel = float(wantedLevel)
showGraph = int(showGraph)

LD,measuredPowers,saveName = parseMeterFile(sourceName)
peakIndices,peakVals,threshold = getPeaks(measuredPowers)
plt.figure(figsize=(17,11))
plotRawGraph(measuredPowers,peakIndices,peakVals,LD) #subplot 1
trimmedInputs,lightout = getCalCoords(peakVals)
plotCalibrationCurve(trimmedInputs,lightout,wantedLevel,LD) #subplot 2

if showGraph>0:
	fig = plt.gcf()
	fig.subplots_adjust(hspace=0.3) #add space between subplots
	plt.show()

getCalVector(wantedLevel,LD,trimmedInputs,lightout)
saveSummary(saveName,wantedLevel,trimmedInputs,lightout,LD)
