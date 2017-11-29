from calFunctions import*
from sys import argv
script, calDataPath,wantedLevel,showGraph = argv

sourceName = calDataPath[1:-1]
wantedLevel = int(wantedLevel)
showGraph = int(showGraph)

LD,trimmedInputs,lightout = parseSummaryFile(sourceName)
plt.figure(figsize=(17,11))
calibrationCurve(trimmedInputs,lightout,wantedLevel,LD) #calibration curve plot

if showGraph>0:
	fig = plt.gcf()
	fig.subplots_adjust(hspace=0.3) #add space between subplots
	plt.show()

getCalVector(wantedLevel,LD,trimmedInputs,lightout)
