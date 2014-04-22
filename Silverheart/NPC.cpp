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
	formal = new std::vector< std::string >;
	path = new std::deque< int >;

	talkTextID = agk::CreateText("planetary");
	agk::SetTextSize(talkTextID, 16);

	talkSID = agk::CloneSprite(1);
	agk::SetSpriteColor(talkSID, 150, 150, 150, 150);
	agk::SetSpriteVisible(talkSID, 0);
	agk::SetTextVisible(talkTextID, 0);

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
			
		}
		else if(state == NPC_passiveState)
		{
			//Clearing the path
			

			//Finding somewhere to walk
			//Gettingt the nodes that the NPC is currently standing on
		}
		else if(state == NPC_walkingState)
		{
			
		}

		if(lastTalk + 4 < globaltime)
		{
			agk::SetSpriteVisible(talkSID, 0);
			agk::SetTextVisible(talkTextID, 0);
		}
		float textWidth = agk::GetTextTotalWidth(talkTextID);
		float textHeight = agk::GetTextTotalHeight(talkTextID);
		agk::SetTextPosition(talkTextID, x - textWidth / 2, y - 150);
		agk::SetSpritePosition(talkSID, x - textWidth / 2 - 5, y - 153);
		agk::SetSpriteScale(talkSID, textWidth + 10, textHeight + 6);

		agk::Print(agk::GetSpriteY(talkSID));

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

void NPC::createFromName(std::string name)
{
	setup();
	//Getting the filename for the character
	std::string filename;
	filename.append(NPC_path);
	filename.append(name);

	if(agk::GetFileExists(filename.data()))
	{
		std::string colSprite;

		//Starting to read the file
		int fileID = agk::OpenToRead(filename.data());
		
		int line = 1; //The line we are currently reading

		while(agk::FileEOF(fileID) != 1)
		{
			char* p; //The pointer which the strings from the file will be added to. Remove content before reading new line
			p = agk::ReadLine(fileID);

			//Getting the type of data on the line
			std::string type;
			type = DataReader::getType(p);
			
			if(line == 1 || type.compare("comment") == 0) //If this is the first line or the line is a comment, skip it
			{

			}
			else
			{
				if(type.compare("error_typeNotFound") == 0) //The line did not contain a type
				{
					DebugConsole::addC("Data error: Type not found (missing ':')");
					DebugConsole::addC(" In file ");
					DebugConsole::addToLog(filename);
				}
				else if(type.compare("Name") == 0) //If this is a name
				{
					//Since an NPC can only have one name, the simple value function is used
					this->name = DataReader::getValue(p);
				}
				else if(type.compare("Formal") == 0)
				{
					int valueAmount = DataReader::getValueAmount(p); //Getting the amount of values

					for(int i = 0; i < valueAmount + 1; i++) //+1 since there are only commas between values and not at the end
					{
						std::string value;
						value = DataReader::getValue(p, i);

						this->formal->push_back(value);
					}
				}
				else if(type.compare("Dialogue") == 0)
				{
					conversationScript = DataReader::getValue(p);
				}
				else if(type.compare("ColSprite") == 0)
				{
					colSprite = DataReader::getValue(p); //Saving the name of the collision sprite for future use
				}
				else if(type.compare("UpdateScript") == 0)
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
		std::string colPath;
		colPath = GF::getPath(colSprite); 
		if(agk::GetFileExists(colPath.data()))
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

	this->name = name;
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
std::string NPC::getName()
{
	return name;
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

bool NPC::hasPath()
{
	if(cPath.size() > 0)
	{
		return true;
	}
	return false;
}
void NPC::setPath(std::vector<PathLink*>* cPath)
{
	if(cPath != NULL)
	{
		this->cPath = *cPath;
	}
}

void NPC::say(std::string msg)
{
	agk::SetTextString(talkTextID, msg.data());

	agk::SetSpriteVisible(talkSID, 1);
	agk::SetTextVisible(talkTextID, 1);

	lastTalk = globaltime;
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
NPC* NPCGroup::getNPCByName(std::string name)
{
	for(unsigned int i = 0; i < npc->size(); i++)
	{
		if(npc->at(i).getName().compare(name) == 0)
		{
			return &npc->at(i);
		}
	}

	return NULL;
}

void NPCGroup::addNPCFromFile(std::string file)
{
	NPC tempNPC;
	tempNPC.createFromName(file);

	//Adding the NPC to the vector of NPCs
	npc->push_back(tempNPC);
}

void NPCGroup::addNPCFromFile(std::string file, float x, float y)
{
	NPCGroup::addNPCFromFile(file);
	npc->back().setPosition(x, y);
}