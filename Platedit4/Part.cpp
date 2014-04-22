#include "Part.h"


Part::Part(void)
{
}


Part::~Part(void)
{
}


void Part::create(int vecID, std::string imgName, float x, float y, int depth)
{
	this->vecID = vecID;

	this->imgName = imgName;
	this->x = x;
	this->y = y;
	this->xNoSnap = x;
	this->yNoSnap = y;
	this->scaleX = 1;
	this->scaleY = 1;
	this->scaleXNoSnap = 1;
	this->scaleYNoSnap = 1;

	this->angle = 0;
	this->depth = depth;
	this->physState = 1;

	this->data = "";

	imgID = agk::LoadImage(imgName.data());
	SID = agk::CreateSprite(imgID);

	agk::SetSpritePositionByOffset(SID, x, y);
	agk::SetSpriteDepth(SID, depth);
	agk::SetSpriteShape(SID, 3);

	agk::SetSpritePhysicsOn(SID, physState);
}
void Part::remove()
{
	if(agk::GetSpriteExists(SID))
	{
		agk::DeleteSprite(SID);
	}
}

int Part::getVecID()
{
	return vecID;
}
bool Part::isHit(float xChk, float yChk)
{
	if(agk::GetSpriteHitTest(SID, xChk, yChk) == 1)
	{
		return true;
	}
	return false;
}

void Part::highlight(int r, int g, int b)
{
	float x1 = agk::WorldToScreenX(agk::GetSpriteX(SID));
	float x2 = agk::WorldToScreenX(agk::GetSpriteX(SID) + agk::GetImageWidth(imgID) * scaleX);
	float y1 = agk::WorldToScreenY(agk::GetSpriteY(SID));
	float y2 = agk::WorldToScreenY(agk::GetSpriteY(SID) + agk::GetImageHeight(imgID) * scaleY);

	agk::DrawLine(x1, y1, x1, y2, r, g, b);
	agk::DrawLine(x1, y2, x2, y2, r, g, b);
	agk::DrawLine(x2, y2, x2, y1, r, g, b);
	agk::DrawLine(x2, y1, x1, y1, r, g, b);
}

void Part::move(float mx, float my)
{
	this->x = x + mx;
	this->y = y + my;
	this->xNoSnap = x;
	this->yNoSnap = y;

	agk::SetSpritePositionByOffset(SID, x, y);
}
void Part::moveSnap(float mx, float my)
{
	xNoSnap = xNoSnap + mx;
	yNoSnap = yNoSnap + my;

	x = floor(xNoSnap / 10) * 10;
	y = floor(yNoSnap / 10) * 10;

	agk::SetSpritePositionByOffset(SID, x, y);
}

void Part::scale(float amountX, float amountY)
{
	scaleX = scaleX * amountX;
	scaleY = scaleY * amountY;

	this->scaleXNoSnap = scaleX;
	this->scaleYNoSnap = scaleY;

	agk::SetSpriteScale(SID, scaleX, scaleY);
}
void Part::scaleSnap(float amountX, float amountY)
{
	scaleXNoSnap = scaleXNoSnap * amountX;
	scaleYNoSnap = scaleYNoSnap * amountY;

	scaleX = floor(scaleXNoSnap * 50) / 50;
	scaleY = floor(scaleYNoSnap * 50) / 50;

	agk::SetSpriteScale(SID, scaleX, scaleY);
}

void Part::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	agk::SetSpritePositionByOffset(SID, x, y);
}
void Part::setAngle(float angle)
{
	this->angle = angle;
	this->angleNoSnap = angle;

	agk::SetSpriteAngle(SID, angle);
}
void Part::setScale(float scaleX, float scaleY)
{
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->scaleXNoSnap = scaleX;
	this->scaleYNoSnap = scaleY;

	agk::SetSpriteScale(SID, scaleX, scaleY);
}
void Part::incPhysState(int amount)
{
	physState = physState + amount;

	if(physState > 2)
	{
		physState = 0;
	}
	if(physState < 0)
	{
		physState = 2;
	}

	agk::SetSpritePhysicsDelete(SID);
	agk::SetSpritePhysicsOn(SID, physState);
	agk::SetSpriteShape(SID, 3);
}
void Part::setPhysState(int state)
{
	physState = state;
	agk::SetSpritePhysicsDelete(SID);
	agk::SetSpritePhysicsOn(SID, physState);
}
void Part::incDepth(int amount)
{
	depth = depth + amount;
	agk::SetSpriteDepth(SID, depth);
}
void Part::setDepth(int depth)
{
	this->depth = depth;
	agk::SetSpriteDepth(SID, depth);
}
void Part::setData(std::string data)
{
	this->data = data;
}
void Part::setColor(int colorR, int colorG, int colorB, int colorA)
{
	this->colorR = colorR;
	this->colorG = colorG;
	this->colorB = colorB;
	this->colorA = colorA;

	agk::SetSpriteColor(SID, colorR, colorG, colorB, colorA);
}

std::string Part::getImgName()
{
	return imgName;
}
float Part::getX()
{
	return x;
}
float Part::getY()
{
	return y;
}
float Part::getAngle()
{
	return angle;
}
float Part::getScaleX()
{
	return scaleX;
}
float Part::getScaleY()
{
	return scaleY;
}
int Part::getPhysState()
{
	return physState;
}
int Part::getDepth()
{
	return depth;
}
std::string Part::getData()
{
	return data;
}