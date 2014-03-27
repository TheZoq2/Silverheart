#pragma once

#include "agk.h"

#include "World.h"

class Character
{
public:
	Character(void);
	~Character(void);

	void create( float sizeX, float sizeY );
	void update();

	void targetSpeed(float speed);
	void jump(float force);

	bool checkOnGround(World* world);
	void setClimbing(bool climbing);

	void setSprite(int SID);
	void setPosition(float x, float y);
	void setImpulse(float centerX, float centerY, float iX, float iY);

	void capYVelocity(float max, float min);
	void setVelocityY(float speed);

	float getX();
	float getY();
	float getFeetX();
	float getFeetY();
private:
	int SID;

	int body;
	int leg;
	int sensor;
	int sides[2];

	float mass;

	float x;
	float y;

	float sizeX;
	float sizeY;

	bool climbing;
};

