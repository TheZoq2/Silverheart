part = getLastActivePart()

targetLevel = getLabelValue(part, 0)
targetPart = getLabelValue(part, 1)

loadNewLevel(targetLevel)
movePlayerToPart(targetPart)

printToConsole("Traveling")