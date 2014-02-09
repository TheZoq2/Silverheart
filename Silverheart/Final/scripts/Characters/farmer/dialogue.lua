function addDialogueToList(windowName, listName, NPC)
	talkingTo = NPC -- The NPC that is 

	-- Creating flags
	if(getNPCFlagExists(NPC, "met") == false) then
		addNPCFlag(NPC, "met", 0);
	end
	if(getNPCFlagExists(NPC, "plrRelation") == false) then
		addNPCFlag(NPC, "plrRelation", 0)
	end


	if(getNPCFlagValue(NPC, "met") == 0) then
		addOptionToList(windowName, listName, "Have we met?", "functions.lua", "greet")
	else
		if( (getNPCFlagValue(NPC, "plrRelation") > -10) and (getNPCFlagValue(NPC, "plrRelation") < 10) ) then
			addOptionToList(windowName, listName, "* Give lots of money *", "functions.lua", "instantLike")

			addOptionToList(windowName, listName, "* Punch *", "functions.lua", "instantHate")
		end
	end

	addOptionToList(windowName, listName, "Goodbye", "functions.lua", "quitDialogue");
end

function addOptionToList(windowName, listName, option, script, func)
	addToSimpleList(windowName, listName, option);
	listItem = getLastListItem(windowName, listName)
	setListItemOnClick(listItem, "scripts/Characters/farmer/" .. script, func);
end