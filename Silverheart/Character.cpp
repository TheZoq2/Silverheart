#include "Character.h"


Character::Character(void)
{
}
Character::~Character(void)
{
}

void Character::create(float sizeX, float sizeY)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	body = agk::CloneSprite(1);
	agk::SetSpriteScale(body, sizeX, sizeY);
	agk::SetSpritePhysicsOn(body, 2);
	agk::SetSpritePhysicsFriction(body, 0);
	agk::SetSpritePhysicsCanRotate(body, 0);
	agk::SetSpriteDepth(body, GF_BaseDepth);

	leg = agk::CloneSprite(1);
	agk::SetSpriteScale(leg, sizeX / 1.4f, sizeX / 1.4f);
	agk::SetSpritePhysicsOn(leg, 2);
	agk::SetSpritePhysicsMass(leg, 0);
	//agk::SetSpritePhysicsFriction(leg, 0.1);
	agk::SetSpriteShape(leg, 1);

	sensor = agk::CloneSprite(1);
	agk::SetSpriteScale(sensor, sizeX / 2, 10);
	agk::SetSpritePhysicsOn(sensor, 2);
	agk::SetSpritePhysicsMass(sensor, 0);
	agk::SetSpritePhysicsIsSensor(sensor, 1);

	agk::SetSpriteCategoryBit(body, 1, 0);
	agk::SetSpriteCategoryBit(leg, 1, 0);
	agk::SetSpriteCategoryBit(body, 2, 1);
	agk::SetSpriteCategoryBit(leg, 2, 1);
	agk::SetSpriteCollideBit(body, 2, 0);
	agk::SetSpriteCollideBit(leg, 2, 0);

	/*for(int i = 0; i < 2; i++)
	{
		sides[i] = agk::CloneSprite(1);
		agk::SetSpriteScale(sides[i], 5, sizeY);
		agk::SetSpritePhysicsOn(sides[i], 2);
		agk::SetSpritePhysicsFriction(sides[i], 0);
		agk::SetSpritePhysicsMass(sides[i], 0);
	}*/

	setPosition(0, 0);

	mass = agk::GetSpritePhysicsMass(body);

	//Creating the joints
	agk::CreateWeldJoint(body, leg, 0, 0, 0);
	agk::CreateWeldJoint(body, sensor, 0, 0, 0);

	//agk::CreateWeldJoint(sides[0], body, -sizeX / 2, sizeY / 2, 0);
	//agk::CreateWeldJoint(sides[1], body, sizeX / 2, sizeY / 2, 0);

	setSprite(1);
}
void Character::update()
{
	this->x = agk::GetSpriteXByOffset(body);
	this->y = agk::GetSpriteYByOffset(body);

	agk::SetSpritePositionByOffset(SID, x, y);
}

void Character::targetSpeed(float speed)
{
	float cVel = agk::GetSpritePhysicsVelocityX(body);
	float velDiff = speed - cVel;
	float impulse = mass * velDiff;

	//Giving the impulse
	setImpulse(x, y, impulse, 0);
}
void Character::jump(float force)
{
	setImpulse(x, y, 0, force);
}

bool Character::checkOnGround(World* world)
{
	if(world->getPhysicsCollision(sensor) != -1)
	{
		return true;
	}

	return false;
}

void Character::setSprite(int SID)
{
	this->SID = agk::CloneSprite(SID);

	agk::SetSpriteScale(this->SID, sizeX / agk::GetImageWidth(agk::GetSpriteImageID(SID)), sizeY / agk::GetImageHeight(agk::GetSpriteImageID(SID)));
	agk::SetSpriteVisible(this->SID, 1);
}
void Character::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	agk::SetSpritePositionByOffset(body, x, y);
	agk::SetSpritePositionByOffset(leg, x, y + sizeY / 2);
	agk::SetSpritePositionByOffset(sensor, x, y + sizeY / 2 + sizeX / 2);

	/*agk::SetSpritePositionByOffset(sides[0], x - sizeX / 2, y);
	agk::SetSpritePositionByOffset(sides[1], x + sizeX / 2, y);*/
}
void Character::setImpulse(float cX, float cY, float iX, float iY)
{
	agk::SetSpritePhysicsImpulse(body, cX, cY, iX, iY);
}

void Character::capYVelocity(float max, float min)
{
	if(agk::GetSpritePhysicsVelocityY(body) > max)
	{
		agk::SetSpritePhysicsVelocity(body, agk::GetSpritePhysicsVelocityX(body), max);
	}
	if(agk::GetSpritePhysicsVelocityY(body) < min)
	{
		agk::SetSpritePhysicsVelocity(body, agk::GetSpritePhysicsVelocityX(body), min);
	}
}
void Character::setVelocityY(float speed)
{
	agk::SetSpritePhysicsVelocity(body, agk::GetSpritePhysicsVelocityY(body), speed);
}

float Character::getX()
{
	return this->x;
}
float Character::getY()
{
	return this->y;
}
float Character::getFeetX()
{
	return agk::GetSpriteXByOffset(sensor);
}
float Character::getFeetY()
{
	return agk::GetSpriteYByOffset(sensor);
}