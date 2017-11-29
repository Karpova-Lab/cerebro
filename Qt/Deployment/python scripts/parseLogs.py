from logFunctions import*
from sys import argv
script, basestationLogPath, cerebroLogPath, showData = argv

baseDF,baseData = parseBaseStation(basestationLogPath)
cerebroDF,cerebroData = parseCerebroLog(cerebroLogPath)
combinedDF,compareData = compare(baseDF,cerebroDF,cerebroData['paramNames'])
summaryStrings = printSummary(combinedDF,baseData['sessionLength'],cerebroData,compareData)

if showData == "alignment and histogram":
    showAlignmentPlot(cerebroLogPath,combinedDF,compareData,summaryStrings)
    showHistogramPlot(combinedDF)
elif showData == "alignment and histogram and outcsv":
    compare(baseDF,cerebroDF,cerebroData['paramNames'],cerebroLogPath)
    showAlignmentPlot(cerebroLogPath,combinedDF,compareData,summaryStrings)
    showHistogramPlot(combinedDF)
elif showData == "save only":
    writeSummary(cerebroLogPath,combinedDF,baseData,cerebroData,compareData)
elif showData == "save and histogram":
    showHistogramPlot(combinedDF)
    writeSummary(cerebroLogPath,combinedDF,baseData,cerebroData,compareData)
