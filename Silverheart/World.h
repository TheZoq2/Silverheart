#pragma once

#include "agk.h"

#include "Part.h"
#include "DebugConsole.h"
#include "zString.h"
#include "DataReader.h"

#include <vector>
#include <list>
#include <string>
#include <random>

class PathNode
{
public:
	void create(int vecID, float x, float y);

	int getVecID();
	float getX();
	float getY();
private:
	float x;
	float y;

	int vecID;
};
class PathLink
{
public:
	void create(int vecID, int node0, int node1, int type);

	int getNodeID(int index);
private:
	int vecID;

	int nodes[2];

	int type;
};

class World
{
public:
	World(void);
	~World(void);

	struct worldSprite //There will be one of these per image in the world. They will be cloned to create the world parts
	{
		std::string file;

		int SID;
		int imgID;
	};

	struct Entry
	{
		std::string name;
		float x;
		float y;
	};

	void begin(); //Used as constructor

	int checkForWS(std::string name); //Will look thru the list of world sprites and return the ID of any existing sprite with the same filename
	int createWS(std::string name);	//Creates a hidden sprite and returns the ID in the worldSprite list

	Entry* findEntry(std::string name);

	void loadBaseMedia();
	void load(std::string name);
	void update(float playerX, float playerY);
	void clear();
	void generateBackgroundTextures();

	void loadV3(char* p, int fileID);
	void loadV4(char* p, int fileID);

	void setOvercast(float overcast);
	void setTime(float time);
	void setCloudLayers(int cloudLayers);
	void setLastActive(int lastActive);

	Part* getPartFromName(std::string name); //This function goes thru all the parts and looks for one with the name specified //It will return the first part with the name
	Part* getPartFromID(int partID);
	int getPartAmount();
	int getPartSID(int partID); //Returns the sprite id of a part in the world
	int getPartPhysState(int partID);
	int getPartUsable(int partID);
	float getPartX(int partID);
	float getPartY(int partID);
	int getSkyID();
	float getTime();
	float getWidth();
	float getHeight();
	float getLeftEdge();
	float getBottomEdge();
	int getLastActive();

	bool isGround(float x, float y);
	int getPhysicsCollision(int sensor);
	
	//int getEntryAmount();
	//Entry* getEntry(int entryID);

	std::string getName();

	//Backdrop
	void loadBG();
	void loadMapAsBg(std::string filename);
	void updateBG(float playerX, float playerY);
	float paralaxOffset(int depth); //Returns the procent that the sprite should be offset based on it's depth

	void setLightModeOn(); //Set to one will set the world in a mode for making light calculations
	void setLightModeOff();

	//Pathfinding functions
	void generateNodes();
	void displayNodes();
	unsigned int getNodeAmount();
	PathNode* findNodeById(int ID);
	PathLink* findLinkById(int ID);


	void addPartToUpdate(Part* part);
private:
	struct Cloud
	{
		int SID;
		float x;
		float y;
		int depth;
	};

	std::string name;

	std::vector< Part >* part;
	std::list< Part* >* partsToUpdate;

	std::vector< Part >* bgParts;
	int bgShader;

	std::vector< worldSprite >* wS;

	std::vector< Entry >* entry;

	int skyID;
	/*unsigned int skyR;
	unsigned int skyG;
	unsigned int skyB;*/
	int skyShader;
	int cloudLayers;

	int bgTexture[8];

	int starAmount;

	struct Star
	{
		int SID;
		float x;
		float y;
	};

	std::vector< Star >* stars;
	std::vector< Cloud >* clouds;

	struct Background
	{
		int SID;
		int imgID;
		float x;
		float y;

		int depth;
	};

	struct CloudBase
	{
		int img;
		int SID;
	};

	std::vector< CloudBase >* cloudBase;
	
	//The size of the world
	float minX;
	float maxX;
	float xDist;

	Background bg[4];
	Background dist[4];

	float overcast;
	float overcastTarget;

	float time;

	int lightSky;

	//Pathfinding variables
	std::vector< PathNode >* nodes;
	std::vector< PathLink >* links;

	int lastActive; //The name of the part that was activated last
};