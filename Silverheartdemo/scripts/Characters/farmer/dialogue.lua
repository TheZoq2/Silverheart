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
		npcSay(NPC, "Hello my good sir, how are you today?");
		
		addOptionToList(windowName, listName, "Have we met?", "functions.lua", "greet")
	else
		if( (getNPCFlagValue(NPC, "plrRelation") > -10) and (getNPCFlagValue(NPC, "plrRelation") < 10) ) then
			addOptionToList(windowName, listName, "* Give lots of money *", "functions.lua", "instantLike")

			addOptionToList(windowName, listName, "* Punch *", "functions.lua", "instantHate")
		end

		if(getNPCFlagValue(NPC, "plrRelation") > 50) then
			addOptionToList(windowName, listName, "Can we make a 'transaction'","functions.lua", "bribe")
		end
	end

	addOptionToList(windowName, listName, "Goodbye", "functions.lua", "quitDialogue");
end

function addOptionToList(windowName, listName, option, script, func)
	addToSimpleList(windowName, listName, option);
	listItem = getLastListItem(windowName, listName)
	setListItemOnClick(listItem, "scripts/Characters/farmer/" .. script, func);
end

---------------------------------------------------------------------------------

---------------------------------------------------------------------------------
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