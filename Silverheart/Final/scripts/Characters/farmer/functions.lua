function quitDialogue()
	removeWindow("DialogWindow");
	setNPCFlagValue(talkingTo, "hasUI", 0)

	setNPCFlagValue(talkingTo, "continueConv", 0)

	talkingTo = nil;
end
function reloadDialogue()
	removeWindow("DialogWindow");
	setNPCFlagValue(talkingTo, "hasUI", 0)

	setNPCFlagValue(talkingTo, "continueConv", 1)
end

function greet()
	if(talkingTo == nil) then
	else
		setNPCFlagValue(talkingTo, "met", 1)
		setNPCFlagValue(talkingTo, "plrRelation", 5)
		npcSay(talkingTo, "Good evening!")

		reloadDialogue()
	end
end

function awnserLocation()
	npcSay(talkingTo, "You are in our secret lab")

	setNPCFlagValue(talkingTo, "whereStage", 1)

	printToConsole(tostring(getNPCFlagValue(talkingTo, "whereStage")))
end
function awnserLab()
	npcSay(talkingTo, "Oh, this is where we produce supersoldiers like you")

	setNPCFlagValue(talkingTo, "whereStage", 2)
end