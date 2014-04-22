--Getting the last activated part
part = getLastActivePart()

--Getting the labels
movePartName = getLabelValue(part, 0)
movePart = getPartByName(movePartName)

targetPartName = getLabelValue(part, 1)
targetPart = getPartByName(targetPartName)

movePartToPart(movePartName, targetPartName, 2)

--Making it look like a door
scissorPart(movePartName);