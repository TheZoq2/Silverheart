#pragma once

#include <string>
#include <vector>

#include "Part.h"
#include "Input.h"
#include "DataEditor.h"
#include "DebugConsole.h"
#include "UI.h"

#include "zString.h"

#define e_defaultDepth 100
#define tool_select 0
#define tool_place 1
#define tool_move 2
#define tool_scale 3
#define tool_rotate 4
#define tool_createNode 5
#define TOOL_MOVENODE 6
#define TOOL_JUMPLINK 7
#define TOOL_LINK 8

class Link
{
public:
	void create(int vecID, int node1, int node2, int type);

	int getNode(int index);
	int getType();
	int getVecID();
private:
	int vecID;

	int type;
	int nodes[2];
};
class Node
{
public:
	void create(int vecID, float x, float y);

	void setVisible(int visible);
	void setColor(int r, int g, int b, int a);
	void setPositon(float x, float y);
	void move(float xAmount, float yAmount);

	bool getHit(float x, float y);
	int getVecID();
	float getX();
	float getY();

private:
	int SID;

	int vecID;
	
	float x;
	float y;
};

class Editor
{
public:
	Editor(void);
	~Editor(void);

	void setup();
	void update(int tool, bool uiActive);
	void updateSelectedWindow(UI* uiGroup);

	void setSelImage(std::string selImage);

	void saveMap(std::string filename);
	void loadMap(std::string filename);

	void incPhysState(int amount);
	void incDepth(int depth);

	void removeSelected();

	void toggleSnapping();

	void copy();
	void paste(float x, float y);

	void resetZoom();
private:
	void readNodeGroup(std::string group);
	void readLinkGroup(std::string group);

	int nextPart;
	std::string selImage;

	std::vector< Part >* parts;
	std::vector< int >* selParts;

	Part* findPartByID(int ID);
	void removePartWithID(int ID);

	float lastRotation;

	float cameraX;
	float cameraY;
	float cameraZoom;

	int lastSelID;

	//Nodes
	Node* findNodeByID(int ID);

	std::vector< Node >* nodes;
	int nextNode;
	std::vector< Link >* links;
	int nextLink;

	int selNode;
	int lastSelNode;

	bool snapping;

	//Part* clipboard;
	int clipboard;
};

