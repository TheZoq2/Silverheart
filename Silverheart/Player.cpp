#include "Player.h"

#include "Script.h"
#include "LuaHandler.h"

Player::Player(void)
{
}


Player::~Player(void)
{
}

void Player::begin(World* world)
{
	this->world = world;

	inventory = new std::vector< Item >;
}

void Player::load(std::string name)
{
	chr.create(64, 128);

	speed = 500;
	jumpHeight = 15;

	//Creating the activation message
	activateSprite = agk::CloneSprite(1);
	agk::SetSpriteColor(activateSprite, 100, 100, 100, 200);
	agk::SetSpriteDepth(activateSprite, 3);
	agk::FixSpriteToScreen(activateSprite, 1);
	activateText = agk::CreateText("E) Activate");
	agk::FixTextToScreen(activateText, 1);
	agk::SetTextDepth(activateText, 2);
	agk::SetTextSize(activateText, 20);

	weapOffsetX = 0;
	weapOffsetY = -1.5;

	//Loading animation stuff, based on spritesheets untill we get spine support
	
	//Reading the animation file
	int fileID = agk::OpenToRead("Data/Player/Animation");

	std::string spineFile;
	std::string atlasFile;

	while(agk::FileEOF(fileID) == 0)
	{
		char* line = agk::ReadLine(fileID);

		std::string dataType;
		dataType = DataReader::getType(line);

		if(dataType.compare("SpineFile") == 0)
		{
			spineFile = DataReader::getValue(line);
		}
		if(dataType.compare("AtlasImage") == 0)
		{
			atlasFile = DataReader::getValue(line);
		}

		delete[] line;
	}

	agk::CloseFile(fileID);
}

void Player::spawn(std::string name)
{
	//Finding the spawnpoint
	/*World::Entry spawnPoint = *world->findEntry(name);

	setPosition(spawnPoint.x, spawnPoint.y);*/

	//Finding the part with the name of the spawnpoint
	Part* spawnPoint = world->getPartFromName( name );

	//We got the spawnpoint, let's set the player position
	setPosition(spawnPoint->getX(), spawnPoint->getY());

	lastTravel = globaltime;
}

void Player::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	chr.setPosition(x, y);

	//agk::SetSpritePositionByOffset(SID, x, y);
}

void Player::update()
{
	float maxLadderSpeed = 140;

	//Movement

	chr.update();
	this->x = chr.getX();
	this->y = chr.getY();

	//Ladders
	bool isLadder = false;
	if(world->isLadder(chr.getFeetX(), chr.getFeetY()))
	{
		isLadder = true;
	}

	//Jump
	if(Input::up() && (lastJump + 0.1f < globaltime || isLadder == true))
	{
		if(chr.checkOnGround(world))
		{
			chr.jump(-1500);

			lastJump = globaltime;
		}
		if(isLadder)
		{
			chr.setVelocityY(-maxLadderSpeed);
		}
	}

	float targetSpeed = 0;
	if(Input::left())
	{
		targetSpeed = -speed;
	}
	if(Input::right())
	{
		targetSpeed = speed;
	}

	chr.targetSpeed(targetSpeed);

	activation(); //Function for looking thru all the parts to find close usable parts

	//Positioning the camera
	cameraX = x - (agk::GetVirtualWidth() / agk::GetViewZoom() / 2);
	cameraY = y - (agk::GetVirtualHeight() / agk::GetViewZoom() / 2) - 200;

	agk::SetViewOffset(cameraX, cameraY);

	PathLink* closestLink = this->world->findClosestLink(x, y);
	if(closestLink != NULL)
	{
		agk::Print(closestLink->getID());
	}

	//Updating platforms
	world->updatePlrFeet(chr.getFeetX(), chr.getFeetY());

	if(isLadder == true)
	{
		chr.capYVelocity(maxLadderSpeed, -maxLadderSpeed);
	}
}
void Player::updateWeapon(ProjectileGroup* projGroup)
{
	cWeapon.setPosition(x + weapOffsetX, y + weapOffsetY);

	//Aiming
	float targetX = i_mx;
	float targetY = i_my;
	
	cWeapon.targetPos(targetX, targetY);

	if(Input::shoot() == true)
	{
		cWeapon.shoot(projGroup);
	}
}

void Player::activation()
{
	float lowestDist = 90;
	bool partFound = false;
	Part* closestPart = NULL;
	int closestPartID;
	float partDist = lowestDist + 1;

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

			int useRange = 90;
			if(dist < useRange) //Checking if the part is within reach
			{
				partFound = true;
				
				if(dist < lowestDist) //CHecking if this is the closest part found
				{
					lowestDist = dist; //Changing the lowest distance
					partDist = dist;

					closestPart = world->getPartFromID(i); //Changing the lowest part we have found
					closestPartID = i;
				}
			}
		}
	}

	bool npcFound = false;
	NPC* closestNPC = NULL;
	float npcDist = lowestDist + 1;
	
	//Looping thru all the NPCs (Using the)
	for(unsigned int i = 0; i < defaultNPCGroup->getNPCAmount(); i++)
	{
		NPC* npc = defaultNPCGroup->getNPC(i);

		if(npc != NULL)
		{
			float npcX = npc->getX();
			float npcY = npc->getY();

			float distX = npcX - x;
			float distY = npcY - y;
			float dist = agk::Sqrt(distX * distX + distY * distY);

			if(dist < lowestDist)
			{
				npcFound = true;

				lowestDist = dist;
				npcDist = dist;

				closestNPC = npc;
			}
		}
	}

	if((partFound == true && closestPart != NULL) || (npcFound == true && closestNPC != NULL))
	{
		if(partDist < npcDist)
		{
			//Positioning the activation text
			agk::SetTextVisible(activateText, 1);
			agk::SetTextPosition(activateText, agk::WorldToScreenX( closestPart->getX() ), agk::WorldToScreenY( closestPart->getY() ));

			//Changing the text to the use text of the part
			std::string fText;
			fText = i_activateName;
			fText.append(") ");
			fText.append(closestPart->getUseMsg());

			agk::SetTextString(activateText, fText.data());

			//Adding the background
			agk::SetSpritePosition(activateSprite, agk::WorldToScreenX(closestPart->getX()) -5.0f, agk::WorldToScreenY(closestPart->getY()) - 2.5f);
			agk::SetSpriteVisible(activateSprite, 1);
			agk::SetSpriteScale(activateSprite, agk::GetTextTotalWidth(activateText) + 10, agk::GetTextTotalHeight(activateText) + 5);

			//Checking if the part is activated
			if(Input::activate() == true)
			{
				//Getting the script of the item
				std::string actScript;
				actScript = ("scripts/");
				actScript.append(closestPart->getActScript());

				//Saving the part that was activated last for use with labels in lua
				world->setLastActive(closestPartID);

				//Checking if this is a lua script or an old script
				if(actScript.find(".lua") != -1)
				{
					LuaHandler::runScript(actScript);
				}
				else
				{
					//Starting the script
					Script::run(actScript.data(), closestPart, world, this);
				}
			}
		}
		else
		{
			//Positioning the activation text
			agk::SetTextVisible(activateText, 1);
			agk::SetTextPosition(activateText, agk::WorldToScreenX( closestNPC->getX() ), agk::WorldToScreenY( closestNPC->getY() ));

			//Changing the text to the use text of the part
			std::string fText;
			fText = i_activateName;
			fText.append(") ");
			fText.append("talk");

			agk::SetTextString(activateText, fText.data());

			//Adding the background
			agk::SetSpritePosition(activateSprite, agk::WorldToScreenX(closestNPC->getX()) -5.0f, agk::WorldToScreenY(closestNPC->getY()) - 2.5f);
			agk::SetSpriteVisible(activateSprite, 1);
			agk::SetSpriteScale(activateSprite, agk::GetTextTotalWidth(activateText) + 10, agk::GetTextTotalHeight(activateText) + 5);

			//Checking if the part is activated
			if(Input::activate() == true)
			{
				closestNPC->startConversation();
			}
		}
	}
	else //No parts where found, hide the text
	{
		agk::SetTextVisible(activateText, 0);
		agk::SetSpriteVisible(activateSprite, 0);
	}
}

bool Player::checkOnGround()
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
			for(float xChk = -plrWidth / 2; xChk < plrWidth / 2; xChk = xChk + 2)
			{
				if(agk::GetSpriteHitTest(wID, x+ xChk, y + (plrHeight / 2) + 5) == 1)
				{
					canJump = true; //Looks like the player can jump
				}
			}
		}
	}

	return canJump; //Returning the result
}

void Player::addItem(Item item)
{
	Item tempItem;
	tempItem.setItem(item);

	inventory->push_back(tempItem);
}

bool Player::canTravel()
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
void Player::setJustLoaded(bool justLoaded)
{
	this->justLoaded = justLoaded;
}
void Player::setVisible(int visible)
{
	agk::SetSpriteVisible(SID, visible);
}

float Player::getX()
{
	return x;
}
float Player::getY()
{
	return y;
}

int Player::getInvAmount()
{
	return inventory->size();
}
Item Player::getItemFromSlot(int slot)
{
	return inventory->at(slot);
}
void Player::setCurrentWeaponByName(std::string weaponName)
{
	cWeapon.loadWeaponByName(weaponName);
}
