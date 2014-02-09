dofile("scripts/Characters/farmer/dialogue.lua");

plrX, plrY = getPlayerPosition();

NPC = getCurrentNPC();
npcX, npcY = getNPCPosition(NPC)

dist = math.sqrt(math.pow(npcX - plrX, 2) + math.pow(npcY - plrY, 2))

cTime = getGameTime(getGameTime());

-- If the state flag exists
if(getNPCFlagExists(NPC, "mainState") == false) then
	addNPCFlag(NPC, "mainState", 0);
end
if(getNPCFlagExists(NPC, "alert") == false) then
	addNPCFlag(NPC, "alert", 0)
end
if(getNPCFlagExists(NPC, "hasUI") == false) then
	addNPCFlag(NPC, "hasUI", 0)
end


mainState = getNPCFlagValue(NPC, "mainState") == 0
alertLevel = getNPCFlagValue(NPC, "alert") == 0;
if mainState == 0 then -- The NPC has nothing to do
	if alertLevel == 0 then

	end
end

if(getNPCFlagValue(NPC, "hasUI") == 0) then
	if(getWindowExists("DialogWindow") == false) then
		printToConsole("Creating new window");

		addUIWindow("DialogWindow", "1x1.png", 100, 250, 300, 100)
		setUIWindowColor("DialogWindow", 100, 100, 100, 150)
		addSimpleListToWindow("DialogWindow", "diaList", 5, 5, 240, 90, "Header")
		setListClickable("DialogWindow", "diaList", true)

		addDialogueToList("DialogWindow", "diaList", NPC);

		setNPCFlagValue(NPC, "hasUI", 1)
	end
end