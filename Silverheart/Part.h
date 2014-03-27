#pragma once

#include "agk.h"
#include "GF.h"

#include <list>
#include <string>

class Part
{
public:
	Part(void);
	~Part(void);

	void cloneSprite(int cloneSprite);
	void setScale(float scaleX, float scaleY);
	void setPosition(float x, float y);
	void setAngle(float angle);
	void setDepth(int depth);
	void setPhysState(int physState);

	void update(); //Performs all operations that should be performed on the part
	void remove();

	void setVisible(int state);

	int getSID();
	int getPhysState();
	float getX();
	float getY();

	void setName(std::string name);
	void setUsable(int usable);
	void setActScript(std::string activScript);
	void setUseMsg(std::string msg);
	void setLabel(int slot, std::string label);
	void setTarget(float targetX, float targetY);
	void setSpeed(float speed);
	void setDrawPos(float x, float y);//Sets the position of the sprite but not the x/y variables, will cause issues
	void setScissorToCurrent(); //Sets the sprite scissor to the current bounds of the sprite
	void setPlatform(int platform);
	void setLadder(int ladder);

	std::string getName();
	int getUsable();
	std::string getActScript();
	std::string getUseMsg();
	std::string getLabel(int slot);
	bool getHit(float x, float y);
	float getEdgeRadius();
	int getPlatform();
	int getLadder();

	void addOperation(std::string operation);
private:
	int SID;
	std::string file;

	int physState;
	int depth;
	float x;
	float y;
	float scaleX;
	float scaleY;
	float angle;
	int platform; //True if the part is a "platform"
	int ladder; //1 if the part is a ladder

	std::string name;
	int usable;
	std::string activScript;
	std::string useMsg;
	
	std::string label[5];

	std::list< std::string >* operations; //List of things that the part should do each update

	float targetX;
	float targetY;
	float speed;
};