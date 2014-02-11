#include "NPC.h"

#include "DebugConsole.h"
#include "LuaHandler.h"

char NPC_path[128] = "Data/Characters/";

int NPC_noState = 0;
int NPC_passiveState = 1;
int NPC_walkingState = 2;
int NPC_hasGoalState = 3;

NPCGroup* defaultNPCGroup;

NPC::NPC(void)
{
}


NPC::~NPC(void)
{
}

void NPC::setup()
{
	formal = new std::vector< uString >;
	path = new std::deque< int >;

	state = 0;
}
void NPC::update(World* world)
{
	if(exists == true) //Making sure that the NPC exists before doing anything with it
	{
		l_lastNPC = this;
		chr.update(); //Updating the character

		this->x = chr.getX();
		this->y = chr.getY();

		if(state == NPC_noState) //The character does not have anything to do, look for something
		{
			state = NPC_passiveState;
		}
		else if(state == NPC_passiveState)
		{
			//Clearing the path
			path->clear();

			//Finding somewhere to walk
			//Gettingt the nodes that the NPC is currently standing on
		}
		else if(state == NPC_walkingState)
		{
			
		}

		LuaHandler::runScript(updateScript);
	}
}
void NPC::updateChars(std::vector< NPC >* npc, Player* player)
{
	//Getting the distance between the NPC and the player
	float plrXDist = x - player->getX();
	float plrYDist = y - player->getY();

	float plrDist = sqrt(pow(plrXDist, 2) + pow(plrYDist, 2));

	if(state == NPC_passiveState) //The NPC has nothing to do, lets follow the player
	{
		setGoal(player->getX(), player->getY() + 2);

		hasGoal = true;
	}
}

void NPC::startConversation()
{
	LuaHandler::runScript(conversationScript);
}

void NPC::createFromName(uString name)
{
	setup();
	//Getting the filename for the character
	uString filename;
	filename.Append(NPC_path);
	filename.Append(name);

	if(agk::GetFileExists(filename))
	{
		uString colSprite;

		//Starting to read the file
		int fileID = agk::OpenToRead(filename);
		
		int line = 1; //The line we are currently reading

		while(agk::FileEOF(fileID) != 1)
		{
			char* p; //The pointer which the strings from the file will be added to. Remove content before reading new line
			p = agk::ReadLine(fileID);

			//Getting the type of data on the line
			uString type;
			type.SetStr(DataReader::getType(p));
			
			if(line == 1 || type.CompareTo("comment") == 0) //If this is the first line or the line is a comment, skip it
			{

			}
			else
			{
				if(type.CompareTo("error_typeNotFound") == 0) //The line did not contain a type
				{
					DebugConsole::addC("Data error: Type not found (missing ':') at line ");
					uString $line;
					$line.Append(line);
					DebugConsole::addC($line);
					DebugConsole::addC(" In file ");
					DebugConsole::addToLog(filename);
				}
				else if(type.CompareTo("Name") == 0) //If this is a name
				{
					//Since an NPC can only have one name, the simple value function is used
					this->name.SetStr(DataReader::getValue(p));
				}
				else if(type.CompareTo("Formal") == 0)
				{
					int valueAmount = DataReader::getValueAmount(p); //Getting the amount of values

					for(int i = 0; i < valueAmount + 1; i++) //+1 since there are only commas between values and not at the end
					{
						uString value;
						value.SetStr(DataReader::getValue(p, i));

						this->formal->push_back(value);
					}
				}
				else if(type.CompareTo("Dialogue") == 0)
				{
					conversationScript = DataReader::getValue(p);
				}
				else if(type.CompareTo("ColSprite") == 0)
				{
					colSprite.SetStr(DataReader::getValue(p)); //Saving the name of the collision sprite for future use
				}
				else if(type.CompareTo("UpdateScript") == 0)
				{
					updateScript = DataReader::getValue(p);
				}
			}

			delete[] p; //Removing the string

			line++;
		}
		agk::CloseFile(fileID);

		//All the data has been loaded, using that data to create the character
		
		//Creating the colision sprite
		uString colPath;
		colPath.SetStr( GF::getPath(colSprite) ); 
		if(agk::GetFileExists(colPath))
		{
			chr.create(64, 128);

			exists = true;
		}
		else
		{
			DebugConsole::addC("Could not find file: ");DebugConsole::addC(colPath);
			DebugConsole::addC(" when loading character: ");DebugConsole::addToLog(name);
		}
	}
	else
	{
		//Telling the console that the file didin't exist
		DebugConsole::addC("Failed to load character: ");DebugConsole::addC(name);DebugConsole::addC("  ---  ");DebugConsole::addToLog(filename);
	}
}
void NPC::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	chr.setPosition(x, y);
}

void NPC::setGoal(float goalX, float goalY)
{
	this->goalX = goalX;
	this->goalY = goalY;
}

float NPC::getX()
{
	return chr.getX();
}
float NPC::getY()
{
	return chr.getY();
}

Flag* NPC::getFlag(std::string name)
{
	for(unsigned int i = 0; i < flags.size(); i++)
	{
		if(flags.at(i).getName().compare(name) == 0)
		{
			return &flags.at(i);
		}
	}
	return NULL;
}
void NPC::addFlag(std::string name, int value)
{
	Flag tempFlag;
	tempFlag.create(name, value);
	flags.push_back(tempFlag);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OldCharacter::create(uString colSprite)
{
	this->colimgID = agk::LoadImage(colSprite);

	this->colSID = agk::CreateSprite(colimgID);

	//Temporary scale
	colScale = .04f;
	scaleX = colScale;
	scaleY = colScale;
	agk::SetSpriteScale(colSID, scaleX, scaleY);

	//Setting physics properties
	agk::SetSpritePhysicsOn(colSID, 2);

	agk::SetSpriteShape(colSID, 3);
	agk::SetSpritePhysicsFriction(colSID, 1.0f);
	agk::SetSpritePhysicsRestitution(colSID, 0);
	agk::SetSpritePhysicsMass(colSID, 0.1f);
	agk::SetSpriteDepth(colSID, 15);

	//Preventing collisioin between this and other characters
	agk::SetSpriteCategoryBit(colSID, 1, 0);
	agk::SetSpriteCategoryBit(colSID, GF_charGroup, 1);
	agk::SetSpriteCollideBit(colSID, GF_charGroup, 0);

	jumpHeight = 3;
	cSpeed = 1.0f;
}
void OldCharacter::update(World* world)
{
	//Preventing the collision sprite from falling over
	agk::SetSpriteAngle(colSID, 0);

	isOnGround = checkOnGround(world);

	//Updating the position of the sprite
	x = agk::GetSpriteXByOffset(colSID);
	y = agk::GetSpriteYByOffset(colSID);

	//Making sure that the left/right speed is not to big
	float chkSpeed = 15.0f * cSpeed;
	if(agk::GetSpritePhysicsVelocityX(colSID) > chkSpeed)
	{
		agk::SetSpritePhysicsVelocity(colSID, chkSpeed, agk::GetSpritePhysicsVelocityY(colSID));
	}
	if(agk::GetSpritePhysicsVelocityX(colSID) < -chkSpeed)
	{
		agk::SetSpritePhysicsVelocity(colSID, -chkSpeed, agk::GetSpritePhysicsVelocityY(colSID));
	}
}

void OldCharacter::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	agk::SetSpritePosition(colSID, x, y);
}

float OldCharacter::getX()
{
	return x;
}
float OldCharacter::getY()
{
	return y;
}
float OldCharacter::getFeetY()
{
	//Getting the height of the colision sprite

	float height = agk::GetImageHeight(colimgID) * scaleY;
	float feetY = y + height/2.0f + 0.2f;
	return feetY;
}
float OldCharacter::getFeetX()
{
	return x;
}

void OldCharacter::jump()
{
	if(isOnGround && globaltime > lastJump + 0.1) //Checking if the player can jump
	{
		agk::SetSpritePhysicsImpulse(colSID, x, y, 0, -jumpHeight);

		lastJump = globaltime;
	}
}
void OldCharacter::walkLeft()
{
	float moveForce = 30.0f;
	//agk::SetSpritePhysicsImpulse(SID, x, y, -0.5f, 0);
	agk::SetSpritePhysicsForce(colSID, x, y, -moveForce, 0);

	cSpeed = 0.5f;
}
void OldCharacter::walkRight()
{
	float moveForce = 30.0f;
	agk::SetSpritePhysicsForce(colSID, x, y, moveForce, 0);

	cSpeed = 0.5f;
}

bool OldCharacter::checkOnGround(World* world)
{
	bool canJump = false;
	//Calculating the height of the sprite
	float plrWidth = agk::GetImageWidth(colimgID) * colScale;
	float plrHeight = agk::GetImageHeight(colimgID) * colScale;

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
					canJump = true; //Looks like the character can jump
				}
			}
		}
	}

	return canJump; //Returning the result
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Waypoint::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Waypoint::getX()
{
	return x;
}
float Waypoint::getY()
{
	return y;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Flag::create(std::string name, int value)
{
	this->name = name;
	this->value = value;
}
std::string Flag::getName()
{
	return name;
}
int Flag::getValue()
{
	return value;
}
void Flag::setValue(int value)
{
	this->value = value;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NPCGroup::setup()
{
	npc = new std::vector< NPC >;
}
void NPCGroup::update(World* world)
{
	for(unsigned int i = 0; i < npc->size(); i++)
	{
		npc->at(i).update(world);
	}
}
void NPCGroup::updateChars(NPCGroup* npcGroup, Player* player)
{
	//Going thru all the NPCS and updating them
	for(unsigned int i = 0; i < npc->size(); i++)
	{
		npc->at(i).updateChars(npc, player);
	}
}

NPC* NPCGroup::getLastNPC()
{
	if(npc->size() > 0)
	{
		return &npc->back();
	}

	return NULL;
}
unsigned int NPCGroup::getNPCAmount()
{
	return npc->size();
}
NPC* NPCGroup::getNPC(unsigned int index)
{
	if(index >= 0 && index < npc->size())
	{
		return &npc->at(index);
	}

	return NULL;
}

void NPCGroup::addNPCFromFile(uString file)
{
	NPC tempNPC;
	tempNPC.createFromName(file);

	//Adding the NPC to the vector of NPCs
	npc->push_back(tempNPC);
}

void NPCGroup::addNPCFromFile(uString file, float x, float y)
{
	NPCGroup::addNPCFromFile(file);
	npc->back().setPosition(x, y);
}