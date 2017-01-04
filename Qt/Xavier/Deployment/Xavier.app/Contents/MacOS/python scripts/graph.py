from logFunctions import*
from sys import argv
script, basestationLogPath, cerebroLogPath = argv

basestationLogPath = basestationLogPath[1:-1]
cerebroLogPath = cerebroLogPath[1:-1]

baseDF,baseData = parseBaseStation(basestationLogPath)
cerebroDF,cerebroData = parseCerebroLog(cerebroLogPath)
combinedDF,compareData = compare(baseDF,cerebroDF,cerebroData['paramNames'])
summaryStrings = printSummary(combinedDF,baseData['sessionLength'],cerebroData['paramRanges'],compareData)
matplotlibGraph(cerebroLogPath,combinedDF,compareData,summaryStrings)