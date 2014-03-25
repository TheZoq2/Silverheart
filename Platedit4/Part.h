#pragma once

#include <string>
#include "agk.h"

class Part
{
public:
	Part(void);
	~Part(void);

	void create(int vecID, std::string imgName, float x, float y, int depth);
	void remove();

	void highlight(int r, int g, int b);
	void move(float mx, float my);
	void moveSnap(float mx, float my);
	void scale(float amountX, float amountY);
	void scaleSnap(float amountX, float amountY);

	void setAngle(float angle);
	void setPosition(float x, float y);
	void setScale(float scaleX, float scaleY);
	void incPhysState(int amount);
	void setPhysState(int state);
	void incDepth(int amount);
	void setDepth(int depth);
	void setData(std::string name);
	void setColor(int colorR, int colorG, int colorB, int colorA);

	bool isHit(float xChk, float yChk);

	std::string getImgName();
	int getVecID();
	float getX();
	float getY();
	float getScaleX();
	float getScaleY();
	float getAngle();
	int getPhysState();
	int getDepth();
	std::string getData();
private:
	int vecID;

	int SID;
	int imgID;

	std::string imgName;

	float x;
	float y;
	float xNoSnap;
	float yNoSnap;

	float scaleX;
	float scaleY;
	float scaleXNoSnap;
	float scaleYNoSnap;

	float angle;
	float angleNoSnap;

	int depth;
	int physState;

	int colorR;
	int colorG;
	int colorB;
	int colorA;

	std::string data;
};

