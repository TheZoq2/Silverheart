function quitDialogue()
	removeWindow("DialogWindow");

	talkingTo = nil;
end
function reloadDialogue()
	setNPCFlagValue(talkingTo, "hasUI", 0)

		quitDialogue()
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