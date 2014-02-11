#include "LuaHandler.h"

lua_State* lh_state;

//LUA functions
//Global
int LUA_getGameTime(lua_State* L)
{
	lua_pushnumber(L, globaltime);

	return 1;
}
int LUA_DebugPrint(lua_State* L)
{
	//Getting the parameter to be printed
	std::string printVal = lua_tostring(L, 1);

	DebugConsole::addToLog(printVal.data());

	return 0;
}
int LUA_runFunctionInC(lua_State* L)
{
	lua_CFunction func = lua_tocfunction(L, 1);

	func(L);

	return 0;
}

//Player
Player* l_defaultPlr;
int LUA_playerSpawnAt(lua_State* L)
{
	//Getting the position to spawn the player at
	std::string spawnpoint = lua_tostring(L, 1);

	l_defaultPlr->spawn(spawnpoint.data());

	return true;
}
int LUA_setPlayerWeapon(lua_State* L)
{
	//Getting the name of the weapon
	std::string weapon = lua_tostring(L, 1);

	l_defaultPlr->setCurrentWeaponByName(weapon.data());
	return 0;
}
int LUA_movePlayerToPart(lua_State* L)
{
	//Getting parameters from lua
	std::string partName = lua_tostring(L, 1);

	//Getting the part with that name
	Part* targetPart = l_defaultWorld->getPartFromName(partName.data());

	if(targetPart == NULL) //If the part didnt exist
	{
		DebugConsole::addC("Failed to move player to part: ");DebugConsole::addC(partName.data());
		DebugConsole::addToLog(". Part did not exist");

		return 0;
	}

	l_defaultPlr->setPosition(targetPart->getX(), targetPart->getY());

	return 0;
}
int LUA_getPlayerPosition(lua_State* L)
{
	lua_pushnumber(L, l_defaultPlr->getX());
	lua_pushnumber(L, l_defaultPlr->getY());

	return 2;
}

//World
World* l_defaultWorld;
int LUA_getPartLabelValue(lua_State* L)
{
	//Getting the value of the parameters passed
	int partID = lua_tointeger(L, 1);
	int label = lua_tointeger(L, 2);

	//Getting the part
	Part* cPart = l_defaultWorld->getPartFromID(partID);

	std::string labelValue = cPart->getLabel(label).GetStr();

	lua_pushstring(L, labelValue.data());

	return 1;
}
int LUA_getLastActivePart(lua_State* L)
{
	lua_pushinteger(L, l_defaultWorld->getLastActive());
	//lua_pushstring(L, "this is a string");

	return 1;
}
int LUA_movePartToPart(lua_State* L)
{
	//Getting the part and target names
	std::string partName = lua_tostring(L, 1);
	std::string targetName = lua_tostring(L, 2);
	float speed = float(lua_tonumber(L, 3));

	//Getting the parts to move
	Part* cPart = l_defaultWorld->getPartFromName(partName.data());
	Part* targetPart = l_defaultWorld->getPartFromName(targetName.data());

	//Making sure nothing is NULL
	if(cPart == NULL)
	{
		DebugConsole::addC("Could not move part: "); DebugConsole::addC(partName.data());
		DebugConsole::addToLog(" to target, part did not exist");

		return 0;
	}
	if(targetPart == NULL)
	{
		DebugConsole::addC("Could not move part: "); DebugConsole::addC(partName.data());
		DebugConsole::addC(" to target, target: ");DebugConsole::addC(targetName.data());
		DebugConsole::addToLog(" did not exist");

		return 0;
	}

	cPart->addOperation("moveToTarget"); //Add the moving operation
	cPart->setTarget(targetPart->getX(), targetPart->getY());
	cPart->setSpeed(speed);

	//Adding the part to  the list of parts to update
	l_defaultWorld->addPartToUpdate(cPart);

	return 0;
}
int LUA_loadNewLevel(lua_State* L)
{
	//Getting parameters from lua
	
	std::string worldName = lua_tostring(L, 1);
	//std::string spawnName = lua_tostring(L, 2);

	std::string worldFile = "levels/";
	worldFile.append(worldName);
	//Making sure the level exists
	if(agk::GetFileExists(worldFile.data()) == 0)
	{
		DebugConsole::addC("Failed to load level: ");DebugConsole::addC(worldName.data());
		DebugConsole::addC(". File: ");DebugConsole::addC(worldFile.data());DebugConsole::addToLog(" did not exist");

		return 0;
	}
	
	//Removing the old world
	l_defaultWorld->clear();
	l_defaultWorld->load(worldFile.data());

	return 0;
	//Spawning the player at the spawnpoint
	//Finding the spawnpoint
	/*Part* spawnPart = l_defaultWorld->getPartFromName(spawnName);
	
	if(spawnPart != NULL)
	{
		l_defaultPlr->setPosition(spawnPart->getX(), spawPart->getY());
	}
	else
	{
		DebugConsole::addC("Failed to put player at spawnpoint: ");DebugConsole::addC(spawnName);DebugConsole::addC(" did not exist");
		l_defaultPlr->setPosition(0, 0);
		return 0;
	}

	return 0;*/
}
int LUA_getPartByName(lua_State* L)
{
	std::string name = lua_tostring(L, 1);
	Part* part = l_defaultWorld->getPartFromName(name.data());

	lua_pushlightuserdata(L, part);

	return 0;
}

//NPCs
NPCGroup* l_defaultNPCgroup;
NPC* l_lastNPC;
int LUA_getCurrentNPC(lua_State* L)
{
	lua_pushlightuserdata(L, l_lastNPC);
	return 1;
}
int LUA_loadNPC(lua_State* L)
{
	//Getting the parameters
	std::string NPCName = lua_tostring(L, 1);
	std::string partName = lua_tostring(L, 2);

	//Finding the part with that name
	Part* part = l_defaultWorld->getPartFromName(partName.data());

	float xPos = part->getX();
	float yPos = part->getY();

	l_defaultNPCgroup->addNPCFromFile(NPCName.data(), xPos, yPos);

	//Passing a pointer to the object to lua
	NPC* newNPC = l_defaultNPCgroup->getLastNPC();

	lua_pushlightuserdata(L, newNPC);

	return 1;
}
int LUA_getNPCPos(lua_State* L)
{
	//Getting the first parameter
	NPC* p = (NPC*)lua_touserdata(L, 1);

	lua_pushnumber(L, p->getX());
	lua_pushnumber(L, p->getY());

	return 2;
}

int LUA_addNPCFlag(lua_State* L)
{
	NPC* p = (NPC*) lua_touserdata(L, 1);
	std::string name = lua_tostring(L, 2);
	int value = (int) lua_tonumber(L, 3);

	p->addFlag(name, value);

	return 0;
}
int LUA_getNPCFlagExists(lua_State* L)
{
	NPC* p = (NPC*) lua_touserdata(L, 1);
	std::string name = lua_tostring(L, 2);

	Flag* flag = p->getFlag(name);

	if(flag != NULL)
	{
		lua_pushboolean(L, true);
	}
	else
	{
		lua_pushboolean(L, false);
	}

	return 1;
}
int LUA_getNPCFlagValue(lua_State* L)
{
	NPC* p = (NPC*) lua_touserdata(L, 1);
	std::string name = lua_tostring(L, 2);

	Flag* flag = p->getFlag(name);

	if(flag != NULL)
	{
		lua_pushnumber(L, flag->getValue());
	}
	else
	{
		DebugConsole::addC("Failed to get NPC flag value: ");
		DebugConsole::addC(name.data());
		DebugConsole::addC(" -- Flag did not exist");
	}
	return 1;
}
int LUA_setNPCFlagValue(lua_State* L)
{
	NPC* p = (NPC*) lua_touserdata(L, 1);
	std::string name = lua_tostring(L, 2);
	int value = (int) lua_tonumber(L, 3);

	Flag* flag = p->getFlag(name);
	if(flag != NULL)
	{
		flag->setValue(value);
	}
	else
	{
		DebugConsole::addC("Failed to set flag value -- Flag: ");
		DebugConsole::addC(name.data());
		DebugConsole::addToLog(" did not exist");
		return 0;
	}

	return 0;
}

//UI
UI* l_defaultUI;
int LUA_addUIWindow(lua_State* L)
{
	std::string windowID = lua_tostring(L, 1); //Window ID
	std::string bgName = lua_tostring(L, 2);
	float xPos = (float) lua_tonumber(L, 3);
	float yPos = (float) lua_tonumber(L, 4);
	float scaleX = (float) lua_tonumber(L, 5);
	float scaleY = (float) lua_tonumber(L, 6);

	l_defaultUI->addWindow(windowID, bgName, xPos, yPos, scaleX, scaleY);
	return 0;
}
int LUA_removeWindow(lua_State* L)
{
	std::string windowName = lua_tostring(L, 1);

	l_defaultUI->markWindowForRemoval(windowName);

	return 0;
}
int LUA_setUIWindowColor(lua_State* L)
{
	std::string name = lua_tostring(L, 1);
	int r = lua_tointeger(L, 2);
	int g = lua_tointeger(L, 2);
	int b = lua_tointeger(L, 2);
	int a = lua_tointeger(L, 2);

	l_defaultUI->setWindowColor(name, r, g, b, a);

	return 0;
}
int LUA_addSimpleListToWindow(lua_State* L)
{
	std::string windowName = lua_tostring(L, 1);
	std::string listName = lua_tostring(L, 2);
	float x = (float) lua_tonumber(L, 3);
	float y = (float) lua_tonumber(L, 4);
	float width = (float) lua_tonumber(L, 5);
	float height = (float) lua_tonumber(L, 6);
	std::string header = lua_tostring(L, 7);
	
	l_defaultUI->addSimpleListToWindow(windowName, listName, x, y, width, height, header);

	return 0;
}
int LUA_addToSimpleList(lua_State* L)
{
	std::string windowName = lua_tostring(L, 1);
	std::string listName = lua_tostring(L, 2);
	std::string value = lua_tostring(L, 3);

	l_defaultUI->addToSimpleList(windowName, listName, value);
	return 0;
}
int LUA_setListClickable(lua_State* L)
{
	std::string windowID = (std::string) lua_tostring(L, 1);//Window ID
	std::string listID = (std::string) lua_tostring(L, 2);
	bool clickable = (bool) lua_toboolean(L, 3);

	Window* window = l_defaultUI->getWindowByID(windowID);
	List* list = window->findListByID(listID);
	list->setClickable(clickable);

	return 0;
}
int LUA_getLastListItem(lua_State* L)
{
	std::string windowID = (std::string) lua_tostring(L, 1);//Window ID
	std::string listID = (std::string) lua_tostring(L, 2);

	Window* window = l_defaultUI->getWindowByID(windowID);
	List* list = window->findListByID(listID);

	lua_pushlightuserdata(L, list->getLastItem());

	return 1;
}
int LUA_setListItemOnClick(lua_State* L)
{
	ListItem* listItem = (ListItem*) lua_touserdata(L, 1);
	std::string file = (std::string) lua_tostring(L, 2);
	std::string function = (std::string) lua_tostring(L, 3);

	if(listItem != NULL)
	{
		listItem->setOnClick(file, function);
	}
	else
	{
		DebugConsole::addC("Failed to set list item on click -- list item is NULL");
	}

	return 0;
}
int LUA_getWindowExists(lua_State* L)
{
	std::string name = lua_tostring(L, 1);

	lua_pushboolean(L, l_defaultUI->getWindowExists(name));

	return 1;
}

LuaHandler::LuaHandler(void)
{
}
LuaHandler::~LuaHandler(void)
{
}

void LuaHandler::setupLua()
{
	lh_state = luaL_newstate();
	luaL_openlibs(lh_state);

	registerFunction("getGameTime", LUA_getGameTime);
	registerFunction("runFunctionInC", LUA_runFunctionInC);

	registerFunction("loadNewLevel", LUA_loadNewLevel);

	lua_register(lh_state, "printToConsole", LUA_DebugPrint);
	registerFunction("playerSpawnAt", LUA_playerSpawnAt);
	registerFunction("setPlayerWeapon", LUA_setPlayerWeapon);
	registerFunction("movePlayerToPart", LUA_movePlayerToPart);
	registerFunction("getPlayerPosition", LUA_getPlayerPosition);

	registerFunction("getLastActivePart", LUA_getLastActivePart);
	registerFunction("getLabelValue", LUA_getPartLabelValue);
	registerFunction("movePartToPart", LUA_movePartToPart);
	registerFunction("getPartByName", LUA_getPartByName);

	registerFunction("createNPC", LUA_loadNPC);
	registerFunction("getCurrentNPC", LUA_getCurrentNPC);
	registerFunction("getNPCPosition", LUA_getNPCPos);

	registerFunction("addNPCFlag", LUA_addNPCFlag);
	registerFunction("getNPCFlagExists", LUA_getNPCFlagExists);
	registerFunction("getNPCFlagValue", LUA_getNPCFlagValue);
	registerFunction("setNPCFlagValue", LUA_setNPCFlagValue);

	registerFunction("addUIWindow", LUA_addUIWindow);
	registerFunction("removeWindow", LUA_removeWindow);
	registerFunction("setUIWindowColor", LUA_setUIWindowColor);
	registerFunction("addSimpleListToWindow", LUA_addSimpleListToWindow);
	registerFunction("addToSimpleList", LUA_addToSimpleList);
	registerFunction("setListClickable", LUA_setListClickable);
	registerFunction("getLastListItem", LUA_getLastListItem);
	registerFunction("setListItemOnClick", LUA_setListItemOnClick);
	registerFunction("getWindowExists", LUA_getWindowExists);
}

void LuaHandler::registerFunction(std::string name, lua_CFunction function)
{
	lua_register(lh_state, name.data(), function);
}

void LuaHandler::runScript(std::string name)
{
	if(luaL_dofile(lh_state, name.data()))
	{
		printError(lh_state, "Failed to run .lua file: ");
	}
}

void LuaHandler::printError(lua_State *L, char* msg)
{
	char buffer[2048];
	sprintf(buffer, "\nFATAL ERROR:\n  %s: %s\n\n", msg, lua_tostring(L, -1));

	DebugConsole::addToLog(buffer);
}

void LuaHandler::runFunction(std::string script, std::string function)
{
	//Run the lua file
	runScript(script);

	//Getting the function
	lua_getglobal(lh_state, function.data());
	
	if(lua_isfunction(lh_state, 1) == true)
	{
		lua_call(lh_state, 0, 0);
	}
	else
	{
		DebugConsole::addC("Failed to run lua function: ");DebugConsole::addC(function.data());
		DebugConsole::addC(" -- Function does not exist in script: ");DebugConsole::addToLog(script.data());
	}
}