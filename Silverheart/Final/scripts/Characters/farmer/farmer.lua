--dofile("scripts/Characters/farmer/dialogue.lua");

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
if(getNPCFlagExists(NPC, "continueConv") == false) then
	addNPCFlag(NPC, "continueConv", 0)
end


mainState = getNPCFlagValue(NPC, "mainState")
alertLevel = getNPCFlagValue(NPC, "alert")
if mainState == 0 then -- The NPC has nothing to do
	if alertLevel == 0 then
		if(dist < 200) then
			printToConsole("Found player")

			--setNPCFlagValue(NPC, "alert", 1)
		end
	elseif(alertLevel == 1) then
		 findNPCPath(NPC, plrX, plrY);
	end
end

if(getNPCFlagValue(NPC, "continueConv") == 1) then
	dofile("scripts/characters/farmer/dialogue.lua")
end