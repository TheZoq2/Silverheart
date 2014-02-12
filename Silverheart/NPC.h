#pragma once

#ifndef NPC_GLOBALS
#define NPC_GLOBALS
	extern char NPC_path[128];

	extern int NPC_noState;
	extern int NPC_passiveState;
	extern int NPC_walkingState;
#endif

#include <vector>
#include <deque>

#include "agk.h"
#include "DataReader.h"
#include "World.h"
#include "GF.h"
#include "Player.h"
#include "Character.h"

class OldCharacter
{
public:
	void create(std::string colSprite);
	void update(World* world);

	bool checkOnGround(World* world);

	//These functions should be called if the character should do something
	void jump();
	void walkLeft();
	void walkRight();

	void setPosition(float x, float y);

	float getX();
	float getY();
	float getFeetY();
	float getFeetX();
private:
	int SID;
	int colSID;
	int colimgID;

	float x;
	float y;

	float scaleX;
	float scaleY;

	float colScale;

	float lastJump;
	float jumpHeight;
	bool isOnGround;

	float cSpeed;
};

class Waypoint
{
public:
	float getX();
	float getY();

	void setPosition(float x, float y);
private:
	float x;
	float y;
};
class Flag
{
public:
	void create(std::string name, int value);

	std::string getName();
	int getValue();
	void setValue(int);
private:
	std::string name;
	int value;
};

class NPC
{
public:
	NPC(void);
	~NPC(void);

	void setup();
	void update(World* world);
	void updateChars(std::vector< NPC >* npc, Player* player);
	void createFromName(std::string name);

	void startConversation();

	void setPosition(float x, float y);

	void setGoal(float goalX, float goalY);

	float getX();
	float getY();
	
	void addFlag(std::string name, int value);
	Flag* getFlag(std::string name);

	//bool findPathToGoal(World* world);

	//NodeLink findClosestNodes(World* world);
private:
	bool exists;

	std::string name;

	std::vector< std::string >* formal;

	Character chr;

	float x;
	float y;

	int state; //The current state of the NPC

	std::deque< int >* path;

	float goalX;
	float goalY;

	bool hasGoal;

	std::string updateScript;
	std::string conversationScript;

	std::vector< Flag > flags;
};

class listElement
{
public:
	int node;

	int state;
	int parent;

	float FScore;
	float HScore;
	float GScore;
};

class NPCGroup
{
public:
	void setup();
	void update(World* world); //Main update loop for NPCs
	void updateChars(NPCGroup* npcGroup, Player* player);

	void addNPCFromFile(std::string file);
	void addNPCFromFile(std::string file, float x, float y);

	NPC* getLastNPC();

	unsigned int getNPCAmount();
	NPC* getNPC(unsigned int index);
private:
	std::vector< NPC >* npc;
};

extern NPCGroup* defaultNPCGroup;