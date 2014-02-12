#pragma once

#include "agk.h"
#include "World.h"
#include "Player.h"
#include "Part.h"
#include "IngameMenu.h"
#include "NPC.h"

#include <vector>
//#include <algorithm>

class Script
{
public:
	Script(void);
	~Script(void);

	static void run(std::string scriptName, Part* part, World* world, Player* player); //Scriptname - Script to run, Part - The part that started the script, Pointers - what should be affected by the script

	static void runFunction(std::string function, Part* part, World* world, Player* player, NPCGroup* npcGroup);
	static void runFunction(std::string function, Part* part, World* world, Player* player);

	static bool isSpecial(std::string param);
	static bool isLabel(std::string param); //Will check if the parameter is a label
	static std::string getValueFromLabel(std::string param, Part* part);

	static std::string getParam(std::string cmd, int number, bool isLast, Part* part);
};

