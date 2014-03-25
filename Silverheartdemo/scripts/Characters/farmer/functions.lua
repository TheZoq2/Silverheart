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

		reloadDialogue()
	end
end

function instantLike()
	if(talkingTo == nil) then
	else
		setNPCFlagValue(talkingTo, "plrRelation", getNPCFlagValue(talkingTo, "plrRelation") + 100);

		reloadDialogue()
	end
end

function instantHate()
	if(talkingTo == nil) then
	else
		setNPCFlagValue(talkingTo, "plrRelation", getNPCFlagValue(talkingTo, "plrRelation") - 100);

		reloadDialogue()
	end
end

function bribe()
	speed = 2 -- The speed that the part should move at

	movePartToPart("door", "door1Target", speed)
end