#include "enemy.h"

#include "Script.h"
Enemy::Enemy(void)
{
}


Enemy::~Enemy(void)
{
}

void Enemy::begin(World* world)
{
	this->world = world;

}

void Enemy::load(uString name)
{
	img = agk::LoadImage(name); //Loading the sprite
	SID = agk::CreateSprite(img);

	spriteScale = .04f;

	agk::SetSpriteScale(SID, spriteScale, spriteScale); //Setting the position

	agk::SetSpritePhysicsOn(SID, 2);

	//agk::SetSpritePhysicsAngularDamping(SID, .999);
	agk::SetSpriteShape(SID, 3);
	agk::SetSpritePhysicsFriction(SID, 1.0f);
	agk::SetSpritePhysicsRestitution(SID, 0);
	agk::SetSpritePhysicsMass(SID, 0.1f);
	agk::SetSpriteDepth(SID, 15);

	speed = 15;
	jumpHeight = 3;

	activateText = agk::CreateText("E) Activate");
	agk::FixTextToScreen(activateText, 1);
	agk::SetTextDepth(activateText, 2);
	agk::SetTextSize(activateText, 20);
}

void Enemy::spawn(uString name)
{
	//Finding the spawnpoint
	/*World::Entry spawnPoint = *world->findEntry(name);

	setPosition(spawnPoint.x, spawnPoint.y);*/
	//We got the spawnpoint, let's set the Enemy position
	setPosition(100,0);

	lastTravel = globaltime;
}

void Enemy::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	agk::SetSpritePositionByOffset(SID, x, y);
}

void Enemy::update()
{
	bool onGround = checkOnGround(); //Checking if the Enemy is on the ground right now

	//Making sure the Enemy does not fall over
	agk::SetSpriteAngle(SID, 0);

	x = agk::GetSpriteXByOffset(SID);
	y = agk::GetSpriteYByOffset(SID);

	if(i_up)
	{
		if(onGround && globaltime > lastJump + 0.1) //Checking if the Enemy can jump
		{
			agk::SetSpritePhysicsImpulse(SID, x, y, 0, -jumpHeight);

			lastJump = globaltime;
		}
	}

	bool moving = false; //Checking if the Enemy is moving this frame
	//Going left/right
	float moveForce = 30.0f;

	if(onGround == false)
	{
		//The Enemy is in the air and can not move as much
		moveForce = 10.0f;
	}

	if(Input::left() == true)
	{
		//agk::SetSpritePhysicsImpulse(SID, x, y, -0.5f, 0);
		agk::SetSpritePhysicsForce(SID, x, y, -moveForce, 0);

		moving = true;
	}
	
	if(Input::right() == true)
	{
		//agk::SetSpritePhysicsImpulse(SID, x, y, 0.5f, 0);
		agk::SetSpritePhysicsForce(SID, x, y, moveForce, 0);

		moving = true;
	}

	//Adapting the speed checking for use with floats to move
	float chkSpeed = speed;

	if(i_moveX != 0)
	{
		chkSpeed = abs(speed * i_moveX);

		agk::SetSpritePhysicsForce(SID, x, y, moveForce * i_moveX, 0);
		moving = true;
	}
	
	if(moving == false) //The Enemy didn't move
	{
		//Checking if the Enemy is on ground
		if(onGround)
		{
			//Stopping the movment
			if(agk::GetSpritePhysicsVelocityX(SID) > 0) //If the velocity is greater than 0
			{
				//agk::SetSpritePhysicsImpulse(SID, x, y, -1.0f, 0);
				agk::SetSpritePhysicsVelocity(SID, 0, agk::GetSpritePhysicsVelocityY(SID));
			}
			else if(agk::GetSpritePhysicsVelocityX(SID) < 0) //If it is lower than 0
			{
				//agk::SetSpritePhysicsImpulse(SID, x, y, 1.0f, 0);
				agk::SetSpritePhysicsVelocity(SID, 0, agk::GetSpritePhysicsVelocityY(SID));
			}
		}
	}

	//Making sure that the left/right speed is not to big
	if(agk::GetSpritePhysicsVelocityX(SID) > chkSpeed)
	{
		agk::SetSpritePhysicsVelocity(SID, chkSpeed, agk::GetSpritePhysicsVelocityY(SID));
	}
	if(agk::GetSpritePhysicsVelocityX(SID) < -chkSpeed)
	{
		agk::SetSpritePhysicsVelocity(SID, -chkSpeed, agk::GetSpritePhysicsVelocityY(SID));
	}

	/*
	//Going thru the entrances to check if any are close
	bool closeToEntry = false;

	for(int i = 0; i < world->getEntryAmount(); i++)
	{
		World::Entry* cEntry = world->getEntry(i);

		//Getting the distances
		float xDist = x - cEntry->x;
		float yDist = y - cEntry->y;

		float dist = agk::Sqrt(xDist*xDist + yDist*yDist);

		if(dist < 6)
		{
			agk::SetTextPosition(activateText, agk::WorldToScreenX(cEntry->x), agk::WorldToScreenY(cEntry->y));

			//Showing the text
			agk::SetTextVisible(activateText, 1);

			//Giving the text a value
			uString actString;
			actString.SetStr("E) Go to ");actString.Append(cEntry->name);
			agk::SetTextString(activateText, actString);

			closeToEntry = true;

			//Checking if the user wants to travel to the new area
			if(i_activate == true && justLoaded == false)
			{
				uString oldName = world->getName();

				world->clear(); //Clearing the old world to make room for a new

				uString newName;

				newName.SetStr("levels/");
				newName.Append(cEntry->name.GetStr());

				if(agk::GetFileExists(newName) == 1 )
				{
					world->load(newName);

					this->spawn(oldName); //Making the Enemy respawn at the new world

					justLoaded = true;
				}
			}
		}
	}

	if(closeToEntry == false) //There were no nearby entries, hide the text
	{
		agk::SetTextVisible(activateText, 0);
	}

	agk::Print(x);
	agk::Print(y);
	*/

	//

	//activation(); //Function for looking thru all the parts to find close usable parts

	//Positioning the camera
	//cameraX = x - (agk::GetDeviceWidth() / agk::GetViewZoom() / 2);
	//cameraY = y - (agk::GetDeviceHeight() / agk::GetViewZoom() / 2);

	//agk::SetViewOffset(cameraX, cameraY);
}

/*void Enemy::activation()
{
	float lowestDist = 6;
	bool partFound = false;
	Part* closestPart;

	//Looping thru all of the parts
	for(int i = 0; i < world->getPartAmount(); i++)
	{
		if(world->getPartUsable(i) == 1) //Making sure we are only looking at parts that can actually be used
		{
			float partX = world->getPartX(i);
			float partY = world->getPartY(i);

			float distX = partX - x;
			float distY = partY - y;
			float dist = agk::Sqrt(distX * distX + distY * distY);

			int useRange = 6;
			if(dist < useRange) //Checking if the part is within reach
			{
				partFound = true;
				
				if(dist < lowestDist) //CHecking if this is the closest part found
				{
					lowestDist = dist; //Changing the lowest distance

					closestPart = world->getPartFromID(i); //Changing the lowest part we have found
				}
			}
		}
	}

	if(partFound == true)
	{
		agk::SetTextVisible(activateText, 1);
		agk::SetTextPosition(activateText, agk::WorldToScreenX( closestPart->getX() ), agk::WorldToScreenY( closestPart->getY() ));

		//Changing the text to the use text of the part
		uString fText;
		fText.SetStr(i_activateName);
		fText.Append(") ");
		fText.Append(closestPart->getUseMsg());

		agk::SetTextString(activateText, fText);

		//Checking if the part is activated
		if(i_activate == true)
		{
			//Getting the script of the item
			uString actScript;
			actScript.SetStr("scripts/");
			actScript.Append(closestPart->getActScript());

			//Starting the script
			Script::run(actScript, closestPart, world, this);
		}
	}
	else //No parts where found, hide the text
	{
		agk::SetTextVisible(activateText, 0);
	}
}
*/
bool Enemy::checkOnGround()
{
	bool canJump = false;
	//Calculating the height of the sprite
	float plrWidth = agk::GetImageWidth(img) * spriteScale;
	float plrHeight = agk::GetImageHeight(img) * spriteScale;
	for(int i = 0; i < world->getPartAmount(); i++)
	{
		int physState = world->getPartPhysState(i);
		if(physState != 0) //making sure that the sprite is setup for physics and part of the world
		{
			//Checking if the lower part of the sprite is coliding with anything
			int wID = world->getPartSID(i);
			for(float xChk = -0.5f; xChk < 0.5f; xChk = xChk + 0.1f)
			{
				if(agk::GetSpriteHitTest(wID, x+ xChk, y + (plrHeight / 2) + 0.5f) == 1)
				{
					canJump = true; //Looks like the Enemy can jump
				}
			}
		}
	}

	return canJump; //Returning the result
}

/*void Enemy::addItem(Item item)
{
	Item tempItem;
	tempItem.setItem(item);

	inventory->push_back(tempItem);
}*/

bool Enemy::canTravel()
{
	/*
	if(justLoaded == false)
	{
		return true;
	}
	*/
	if(globaltime + 0.5 > lastTravel)
	{
		return true;
	}
	
	return false;
}
void Enemy::setJustLoaded(bool justLoaded)
{
	this->justLoaded = justLoaded;
}

float Enemy::getX()
{
	return x;
}
float Enemy::getY()
{
	return y;
}

/*int Enemy::getInvAmount()
{
	return inventory->size();
}
Item Enemy::getItemFromSlot(int slot)
{
	return inventory->at(slot);
}*/