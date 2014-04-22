#include "World.h"

#include "LuaHandler.h"

World::World(void)
{
}


World::~World(void)
{
}

void World::begin()
{
	part = new std::vector< Part >;
	partsToUpdate = new std::list< Part* >;
	bgParts = new std::vector< Part >;

	wS = new std::vector< worldSprite >;

	entry = new std::vector< Entry >;

	clouds = new std::vector< Cloud >;

	stars = new std::vector< Star >;

	nodes = new std::vector< PathNode >;
	links = new std::vector< PathLink >;

	cloudLayers = 4;
}

void World::load(std::string filename)
{
	partsToUpdate = new std::list< Part* >;
	//Starting to read the level file
	int fileID = agk::OpenToRead(filename.data());
		//Reading the version of the file
		char* p;//Char* to allow me to remove the string when done
		p = agk::ReadString(fileID);
		std::string version = p;
		delete[] p;

		if(version.compare("1") == 0)
		{
			p = agk::ReadString(fileID); //Special data 
			delete[] p;

			p = agk::ReadString(fileID);
			unsigned int partAmount = agk::Val(p);
			delete[] p;

			for(unsigned int i = 0; i < partAmount; i++)
			{

				agk::ReadString(fileID); //Reading the |
				std::string imgName;
				float xPos = 0;
				float yPos = 0;
				float scaleX = 1;
				float scaleY = 1;
				float angle = 0;
				int physState = 0;
				int depth = GF_BaseDepth;
				int visible = 1;
				int usable = 0;
				std::string useScript = "";
				std::string useMsg = "";
				int platform = 0;
				int ladder = 0;
				std::string labels[5];

				std::string name;

				//Reading the string
				std::string partData = agk::ReadString(fileID);

				std::vector< std::string > dataSegments = zString::split(partData, ";"); //Spliting the data into parts

				for(unsigned int i = 0; i < dataSegments.size(); i++)
				{
					//Splitting into type and value
					std::vector< std::string > dataParts = zString::split(dataSegments.at(i), ":");
					
					//Making sure this is actual data
					if(dataParts.size() == 2)
					{
						std::string dataType = dataParts.at(0);
						std::string dataValue = dataParts.at(1);

						if(dataType.compare("imgName") == 0)
						{
							imgName = dataValue;
						}
						else if(dataType.compare("xPos") == 0)
						{
							xPos = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("yPos") == 0)
						{
							yPos = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("scaleX") == 0)
						{
							scaleX = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("scaleY") == 0)
						{
							scaleY = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("angle") == 0)
						{
							angle = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("physState") == 0)
						{
							physState = agk::Val(dataValue.data());
						}
						else if(dataType.compare("depth") == 0)
						{
							depth = agk::Val(dataValue.data());
						}
						else if(dataType.compare("name") == 0)
						{
							name = dataValue;
						}
						else if(dataType.compare("visible") == 0)
						{
							visible = atoi(dataValue.data());
						}
						else if(dataType.compare("usable") == 0)
						{
							usable = atoi(dataValue.data());
						}
						else if(dataType.compare("useScript") == 0)
						{
							useScript = dataValue;
						}
						else if(dataType.compare("useMsg") == 0)
						{
							useMsg = dataValue;
						}
						else if(dataType.compare("platform") == 0)
						{
							platform = atoi(dataValue.data());
						}
						else if(dataType.compare("ladder") == 0)
						{
							ladder = atoi(dataValue.data());
						}
						else if(dataType.find("label") != std::string::npos)
						{
							std::string labelIDStr = dataType.substr(5, std::string::npos);
							int labelID = atoi(labelIDStr.data());

							if(labelID >= 0 && labelID < 5)
							{
								labels[labelID] = dataValue.data();
							}
							else
							{
								DebugConsole::addC("Failed to set part label, ID is not in bounds");
							}
						}
						else
						{
							DebugConsole::addC("Unrecognised datatype: ");DebugConsole::addC(dataType.data());
							DebugConsole::addC(" when loading level: ");DebugConsole::addToLog(filename);
						}
					}
				}

				Part tempPart;

				//Checking if the sprite exists
				if(agk::GetFileExists(imgName.data()) == 0)
				{
					tempPart.cloneSprite(1);
				}
				else
				{
					int cloneSprite = checkForWS(imgName.data());

					if(cloneSprite == 0)
					{
						int ID = createWS(imgName.data());

						tempPart.cloneSprite(wS->at(ID).SID);
					}
					else
					{
						tempPart.cloneSprite(wS->at(cloneSprite).SID);
					}
				}
					

				tempPart.setScale(scaleX, scaleY);
				tempPart.setPosition(xPos, yPos);
				tempPart.setAngle(angle);
				tempPart.setDepth(depth);
				tempPart.setPhysState(physState);
				tempPart.setVisible(visible);

				tempPart.setName(name.data());

				tempPart.setActScript(useScript.data());
				tempPart.setUsable(usable);
				tempPart.setUseMsg(useMsg.data());

				tempPart.setPlatform(platform);
				tempPart.setLadder(ladder);
				
				for(unsigned int i = 0; i < 5; i++)
				{
					tempPart.setLabel(i, labels[i]);
				}

				this->part->push_back(tempPart);
				
			}

		}
		else
		{
			DebugConsole::addC("Failed to load level: ");DebugConsole::addC(filename.data());
			DebugConsole::addC(", invalid version: ");DebugConsole::addToLog(p);
		}
	
		//Reading additional data
		while(agk::FileEOF(fileID) == 0)
		{
			char * p;
			p = agk::ReadString(fileID);
			std::string str = p;
			delete[] p;

			//Splitting the string at the = sign
			std::vector< std::string > data = zString::split(str, "=");
			
			if(data.size() == 2)
			{
				if(data.at(0).compare("nodes") == 0)
				{
					std::vector< std::string > arrays = DataReader::getArray(data.at(1));

					for(unsigned int i = 0; i < arrays.size(); i++)
					{
						std::vector<std::string> variables = zString::split(arrays.at(i), ",");

						int vecID;
						float x;
						float y;

						for(unsigned int n = 0; n < variables.size(); n++)
						{
							std::string dataType = DataReader::getType(variables.at(n).data());
							std::string dataValue = DataReader::getValue(variables.at(n).data(), 0);

							if(dataType.compare("vecID") == 0)
							{
								vecID = atoi(dataValue.data());
							}
							if(dataType.compare("x") == 0)
							{
								x = (float) atof(dataValue.data());
							}
							if(dataType.compare("y") == 0)
							{
								y = (float) atof(dataValue.data());
							}
						}

						PathNode tempNode;
						tempNode.create(vecID, x, y);
						nodes->push_back(tempNode);
					}
				}

				if(data.at(0).compare("links") == 0)
				{
					std::vector< std::string > arrays = DataReader::getArray(data.at(1));

					for(unsigned int i = 0; i < arrays.size(); i++)
					{
						std::vector<std::string> variables = zString::split(arrays.at(i), ",");

						int vecID;
						int node0;
						int node1;
						int type;

						for(unsigned int n = 0; n < variables.size(); n++)
						{
							std::string dataType = DataReader::getType(variables.at(n).data());
							std::string dataValue = DataReader::getValue(variables.at(n).data(), 0);

							if(dataType.compare("vecID") == 0)
							{
								vecID = atoi(dataValue.data());
							}
							if(dataType.compare("node0") == 0)
							{
								node0 = atoi(dataValue.data());
							}
							if(dataType.compare("node1") == 0)
							{
								node1 = atoi(dataValue.data());
							}
							if(dataType.compare("type") == 0)
							{
								type = atoi(dataValue.data());
							}
						}

						PathLink tempLink;
						tempLink.create(vecID, node0, node1, type);
						links->push_back(tempLink);
					}

					//Going thru all the links and adding them to the nodes that they connect
					for(unsigned int i = 0; i < links->size(); i++)
					{
						PathNode* node0 = findNodeById(links->at(i).getNodeID(0));
						PathNode* node1 = findNodeById(links->at(i).getNodeID(1));

						node0->addLink(links->at(i).getID());
						node1->addLink(links->at(i).getID());
					}
				}
			}
		}

	agk::CloseFile(fileID);

	loadBG();

	//Runing the level onload script
	std::string onload = "scripts/";
	onload.append(filename);
	onload.append("/onload.lua");
	LuaHandler::runScript(onload);
}

void World::update(float playerX, float playerY)
{
	updateBG(playerX, playerY);

	//Updating parts that should be updated
	//making sure the list has atleast one element
	if(partsToUpdate->size() != 0)
	{
		std::list< Part* >::iterator it; //Iterator for looping thru parts
		for(it = partsToUpdate->begin(); it != partsToUpdate->end(); it++)
		{
			Part* cPart = *it;
			cPart->update();
		}
	}

	displayNodes();
}
void World::updatePlrFeet(float plrFeetX, float plrFeetY)
{
	//Going thru all the parts that are platforms
	for(unsigned int i = 0; i < part->size(); i++)
	{
		if(part->at(i).getPlatform())
		{
			if(part->at(i).getY() < plrFeetY)
			{
				part->at(i).setPhysState(0);
			}
			else
			{
				part->at(i).setPhysState(2);
			}
		}
	}
}
void World::clear()
{
	//Remvoving the background
	agk::DeleteObject(skyID);

	for(unsigned int i = 0; i < part->size(); i++)
	{
		part->at(i).remove();
	}

	part->clear();
	partsToUpdate->clear();
}

void World::loadBG()
{
	overcast = 0.5;
	overcastTarget = overcast;

	skyID = agk::CreateObjectPlane(100, 100);
	agk::SetObjectLookAt(skyID, 0, 5, 0, 0);
	agk::SetObjectColor(skyID, 255, 0, 0, 255);

	agk::SetCameraPosition(1, 0, 5, 0);
	agk::SetCameraLookAt(1, 0, 0, 0, 0);

	skyShader = agk::LoadShader("shaders/sky.vs", "shaders/sky.fs");
	agk::SetShaderConstantByName(skyShader, "iResolution", float( agk::GetVirtualWidth() ), float( agk::GetVirtualHeight() ), 0, 0);

	float dSkyR = 0.62f;
	float dSkyG = 0.78f;
	float dSkyB = 1.0f;

	float nSkyR = 0.05f;
	float nSkyG = 0.05f;
	float nSkyB = 0.39f;

	agk::SetShaderConstantByName(skyShader, "dSky", dSkyR, dSkyG, dSkyB, 1.0f);
	agk::SetShaderConstantByName(skyShader, "nSky", nSkyR, nSkyG, nSkyB, 1.0f);
	
	float dCloudR = 1.0f;
	float dCloudG = 1.0f;
	float dCloudB = 1.0f;
	
	float nCloudR = 0.05f;
	float nCloudG = 0.05f;
	float nCloudB = 0.05f;

	agk::SetShaderConstantByName(skyShader, "dCloud", dCloudR, dCloudG, dCloudB, 1.0f);
	agk::SetShaderConstantByName(skyShader, "nCloud", nCloudR, nCloudG, nCloudB, 1.0f);

	agk::SetObjectShader(skyID, skyShader);

	
}

void World::generateBackgroundTextures()
{
	int bgTextureAmount = 8;

	//Creating a 3d plane to generate things on
	int plane = agk::CreateObjectPlane(100, 100);
	agk::SetObjectLookAt(plane, 0, 5, 0, 0);
	agk::SetObjectColor(plane, 255, 0, 0, 255);

	agk::SetCameraPosition(1, 0, 5, 0);
	agk::SetCameraLookAt(1, 0, 0, 0, 0);

	//Moving the 2d camera away from everything
	float oldOffsetX = agk::GetViewOffsetX(); float oldOffsetY = agk::GetViewOffsetY();
	agk::SetViewOffset(-100000, -100000);

	for(int i = 0; i < 6; i++) //Generating cloud textures
	{
		
	}

	//Resetting the offset
	agk::SetViewOffset(oldOffsetX, oldOffsetY);
}
void World::updateBG(float playerX, float playerY)
{
	////////////////////////////////////////////////////////////////////////
	//							Time
	////////////////////////////////////////////////////////////////////////
	//Making sure time is between 0 and 24
	while(time > 2400)
	{
		time = time - 2400;
	}
	//Increasing time
	float timeInc = 0.5f * speedMod;
	time = time + timeInc;
	
	//Setting the color values for day/night
	int dayR = 160;
	int dayG = 200;
	int dayB = 255;

	int nightR = 25;
	int nightG = 25;
	int nightB = 100;

	if(isIndoors == false)
	{
		//checking if it is nighttime
		if(time > 2200 || time < 600)
		{
			/*skyR = nightR;
			skyG = nightG;
			skyB = nightB;*/

			//Part color
			for(unsigned int i = 0; i < part->size(); i++)
			{
				agk::SetSpriteColor(part->at(i).getSID(), 5, 5, 5, 255);
			}

			//Star color
			for(unsigned int i = 0; i < stars->size(); i++)
			{
				agk::SetSpriteColor(stars->at(i).SID, 255, 255, 255, 255);
			}
		}
		else if(time > 600 && time < 800)
		{
			//Calculating the diffirence in color levels
			float rDiff = 255 - 5;
			float gDiff = 255 - 5;
			float bDiff = 255 - 5;

			float timeFact = (time - 600) / 200;

			float r = 5 + rDiff * timeFact;
			float g = 5 + gDiff * timeFact;
			float b = 5 + bDiff * timeFact;

			for(unsigned int i = 0; i < part->size(); i++)
			{
				agk::SetSpriteColor(part->at(i).getSID(), int( r ), int( g ), int( b ), 255);
			}
		}
		else if(time > 800 && time < 1999)
		{
			/*skyR = dayR;
			skyG = dayG;
			skyB = dayB;*/

			for(unsigned int i = 0; i < part->size(); i++)
			{
				agk::SetSpriteColor(part->at(i).getSID(), 255, 255, 255, 255);
			}
		}
		else if( time > 2000 && time < 2200)
		{
			//Calculating the diffirence in color levels
			float rDiff = 255 - 5;
			float gDiff = 255 - 5;
			float bDiff = 255 - 5;

			float timeFact = (time - 2000) / 200;

			float r = 255 - rDiff * timeFact;
			float g = 255 - gDiff * timeFact;
			float b = 255 - bDiff * timeFact;

			for(unsigned int i = 0; i < part->size(); i++)
			{
				agk::SetSpriteColor(part->at(i).getSID(), int( r ), int( g ), int( b ), 255);
			}
		}
	}
	
	//Weather change
	float overcastChange = 0.00005f;

	if(abs(overcast - overcastTarget) < overcastChange)
	{
		if(rand() % 1000 == 1)
		{
			//The overcast is done changing, select a new target
			overcastTarget = agk::Random(0, 100) / 100.0f * 0.75f + 0.25f;
		}
	}

	if(overcast > overcastTarget)
	{
		overcast = overcast - overcastChange;
	}
	else
	{
		overcast = overcast + overcastChange;
	}

	agk::SetShaderConstantByName(skyShader, "overcast", overcast / cloudLayers * 4, 0, 0, 0);
	agk::SetShaderConstantByName(skyShader, "posX", playerX / 2000 / 10, 0, 0, 0);
	agk::SetShaderConstantByName(skyShader, "posY", - playerY / 2000 / 10, 0, 0, 0);
	agk::SetShaderConstantByName(skyShader, "cloudLayers", float( cloudLayers ), 5, 5, 5);

	agk::SetShaderConstantByName(skyShader, "time", time, 0, 0, 0);

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//Updating the bagkround layer
	for(unsigned int i = 0; i < bgParts->size(); i++)
	{
		float xPos = bgParts->at(i).getX() + playerX / 2.0f;
		float yPos = bgParts->at(i).getY() + playerY / 2.0f - 700;
		bgParts->at(i).setDrawPos(xPos, yPos);
	}

	if(isIndoors == true)
	{
		agk::SetObjectVisible(skyID, 0);
	}
	else
	{
		agk::SetObjectVisible(skyID, 1);
	}
}

void World::loadV3(char* p, int fileID)
{
	if(strcmp(p, "3") == 0) //This is version 0, continue reading the file
		{
			delete[] p; //Removing the data pointed to by p

			p = agk::ReadString(fileID); //Reading the name of the level
			name = p;
			delete[] p;
			p = agk::ReadString(fileID); delete[] p; //Backgrounds -- Unused
			p = agk::ReadString(fileID); delete[] p; //Backgrounds -- Unused

			//Reading the fun stuff
			p = agk::ReadString(fileID); //Reading the amount of parts in the world
			int amount = atoi(p);
			delete[] p;

			for(int i = 0; i < amount; i++)
			{
				//Creating a temporary variable which will be pushed back into the vector once everything is set up
				Part tempPart;
				//Reading the filename
				//std::string filename = agk::ReadString(fileID);

				std::string filename;
				filename = agk::ReadString(fileID);

				//Checking if the filename already exists
				//Making sure the sprite we are trying to load actually exists

				bool fileExist = true;
				//Making sure that the file exists
				if(agk::GetFileExists(filename.data()) == 0)
				{
					fileExist = false;

					//If it dosn't exist, write that to a debug file
					int debugID = agk::OpenToWrite("debug.txt", 1);
					agk::WriteString(debugID, "\n The sprite ");
					agk::WriteString(debugID, filename.data());
					agk::WriteString(debugID, " didn't exist");
					agk::CloseFile(debugID);
				}

				if(fileExist == true) //Making sure that the file we are loading exists
				{
					int cloneSprite = checkForWS(filename);
					if(cloneSprite == 0) //Checking if the filename exists
					{
						//it didn't exist, we have to create it
						int ID = createWS(filename);

						tempPart.cloneSprite(wS->at(ID).SID);
					}
					else
					{
						//It existed, we can just clone the old sprite
						tempPart.cloneSprite(wS->at(cloneSprite).SID);
					}
				}

				//A sprite has been created, let's set some parameters for it
				p = agk::ReadString(fileID); float x = agk::ValFloat(p); //Reading the x position
				delete[] p;
				p = agk::ReadString(fileID); float y = agk::ValFloat(p);
				delete[] p;

				p = agk::ReadString(fileID); int depth = agk::Val(p); //Reading the depth
				delete[] p;

				p = agk::ReadString(fileID); float scaleX = agk::ValFloat(p); //Reading the scale
				delete[] p;
				p = agk::ReadString(fileID); float scaleY = agk::ValFloat(p);
				delete[] p;

				p = agk::ReadString(fileID); float angle = agk::ValFloat(p); //Reading the angle
				delete[] p;

				p = agk::ReadString(fileID); int physState = agk::Val(p); //Reading the physics state
				delete[] p;


				////////////////////////////////////////////////////////////////////////////////////
				char* name = agk::ReadString(fileID);//Reading the name

				p = agk::ReadString(fileID); int usable = agk::Val(p); //Reading the usability
				delete[] p;

				char* actScript = agk::ReadString(fileID); //Reading the activation script

				char* useMsg =  agk::ReadString(fileID); //Reading the use message
				
				char* labels[5];
				//Reading the labels
				for(int lbl = 0; lbl < 5; lbl++)
				{
					labels[lbl] = agk::ReadString(fileID);
				}

				if(fileExist == true) //Since the file existed, we will set data to the new sprite
				{
					tempPart.setScale(scaleX, scaleY);
					tempPart.setPosition(x, y); //Setting the position
					tempPart.setDepth(depth); //Setting the depth
					tempPart.setAngle(angle);
					tempPart.setPhysState(physState);
					tempPart.setVisible(1);

					//Setting script paramenters
					tempPart.setName( name ); 
					tempPart.setUsable( usable );
					tempPart.setActScript(actScript);
					tempPart.setUseMsg(useMsg);

					for(int lbl = 0; lbl < 5; lbl++)
					{
						tempPart.setLabel(lbl, labels[lbl]);
					}

					part->push_back(tempPart);
				}

				//Cleaning up that garbage
				delete[] name;
				delete[] actScript;
				delete[] useMsg;
				for(int lbl = 0; lbl < 5; lbl++)
				{
					delete[] labels[lbl];
				}
			}

			/*
			//Reading the spawnpoints
			int entryAmount = agk::ReadInteger(fileID);
			for(int i = 0; i < entryAmount; i++)
			{
				std::string name = agk::ReadString(fileID);
				float x = agk::ReadFloat(fileID);
				float y = agk::ReadFloat(fileID);

				Entry tempEntry;
				tempEntry.name.SetStr(name);
				tempEntry.x = x;
				tempEntry.y = y;

				entry->push_back(tempEntry);
			}
			*/
		}
}
void World::loadV4(char* p, int fileID)
{
	delete[] p; //Removing the data pointed to by p

	p = agk::ReadString(fileID); //Reading the name of the level
	name = p;
	delete[] p;
	p = agk::ReadString(fileID); delete[] p; //Backgrounds -- Unused
	p = agk::ReadString(fileID); delete[] p; //Backgrounds -- Unused

	//Reading the fun stuff
	p = agk::ReadString(fileID); //Reading the amount of parts in the world
	int amount = atoi(p);
	delete[] p;

	for(int i = 0; i < amount; i++)
	{
		//Creating a temporary variable which will be pushed back into the vector once everything is set up
		Part tempPart;
		//Reading the filename
		//std::string filename = agk::ReadString(fileID);

		std::string filename;
		filename = (agk::ReadString(fileID));

		//Checking if the filename already exists
		//Making sure the sprite we are trying to load actually exists

		bool fileExist = true;
		//Making sure that the file exists
		if(agk::GetFileExists(filename.data()) == 0)
		{
			fileExist = false;

			//If it dosn't exist, write that to a debug file
			int debugID = agk::OpenToWrite("debug.txt", 1);
			agk::WriteString(debugID, "\n The sprite ");
			agk::WriteString(debugID, filename.data());
			agk::WriteString(debugID, " didn't exist");
			agk::CloseFile(debugID);
		}

		if(fileExist == true) //Making sure that the file we are loading exists
		{
			int cloneSprite = checkForWS(filename);
			if(cloneSprite == 0) //Checking if the filename exists
			{
				//it didn't exist, we have to create it
				int ID = createWS(filename);

				tempPart.cloneSprite(wS->at(ID).SID);
			}
			else
			{
				//It existed, we can just clone the old sprite
				tempPart.cloneSprite(wS->at(cloneSprite).SID);
			}
		}

		//A sprite has been created, let's set some parameters for it
		p = agk::ReadString(fileID); float x = agk::ValFloat(p); //Reading the x position
		delete[] p;
		p = agk::ReadString(fileID); float y = agk::ValFloat(p);
		delete[] p;

		p = agk::ReadString(fileID); int depth = agk::Val(p); //Reading the depth
		delete[] p;

		p = agk::ReadString(fileID); float scaleX = agk::ValFloat(p); //Reading the scale
		delete[] p;
		p = agk::ReadString(fileID); float scaleY = agk::ValFloat(p);
		delete[] p;

		p = agk::ReadString(fileID); float angle = agk::ValFloat(p); //Reading the angle
		delete[] p;

		p = agk::ReadString(fileID); int physState = agk::Val(p); //Reading the physics state
		delete[] p;


		////////////////////////////////////////////////////////////////////////////////////
		char* name = agk::ReadString(fileID);//Reading the name

		p = agk::ReadString(fileID); int usable = agk::Val(p); //Reading the usability
		delete[] p;

		char* actScript = agk::ReadString(fileID); //Reading the activation script

		char* useMsg =  agk::ReadString(fileID); //Reading the use message
				
		char* labels[5];
		//Reading the labels
		for(int lbl = 0; lbl < 5; lbl++)
		{
			labels[lbl] = agk::ReadString(fileID);
		}

		if(fileExist == true) //Since the file existed, we will set data to the new sprite
		{
			tempPart.setScale(scaleX, scaleY);
			tempPart.setPosition(x, y); //Setting the position
			tempPart.setDepth(depth); //Setting the depth
			tempPart.setAngle(angle);
			tempPart.setPhysState(physState);
			tempPart.setVisible(1);

			//Setting script paramenters
			tempPart.setName( name ); 
			tempPart.setUsable( usable );
			tempPart.setActScript(actScript);
			tempPart.setUseMsg(useMsg);

			for(int lbl = 0; lbl < 5; lbl++)
			{
				tempPart.setLabel(lbl, labels[lbl]);
			}

			part->push_back(tempPart);
		}

		//Cleaning up that garbage
		delete[] name;
		delete[] actScript;
		delete[] useMsg;
		for(int lbl = 0; lbl < 5; lbl++)
		{
			delete[] labels[lbl];
		}
	}

	/*//Reading pathfinding nodes
	int nodeAmount = agk::ReadInteger(fileID);
	for(int i = 0; i < nodeAmount; i++)
	{
		PathNode tempNode;

		int ID = agk::ReadInteger(fileID);
		float xPos = agk::ReadFloat(fileID);
		float yPos = agk::ReadFloat(fileID);

		tempNode.create(ID, xPos, yPos);

		int linkAmount = agk::ReadInteger(fileID);
		for(int n = 0; n < linkAmount; n++)
		{
			int linkID = agk::ReadInteger(fileID);
			int linkType = agk::ReadInteger(fileID);

			//tempNode.addLink(linkID, linkType);
		}

		//Adding the new node to the vector
		node->push_back(tempNode);
	}*/
}

int World::checkForWS(std::string filename)
{
	int ID = 0;

	for(unsigned int i = 0; i < wS->size(); i++)
	{
		//if(strcmp(wS->at(i).file.GetStr(), filename.GetStr()) == 0)
		if(wS->at(i).file.compare(filename) == 0)
		{
			ID = i;
		}
	}

	return ID;
}

int World::createWS(std::string filename)
{
	int ID;

	ID = wS->size(); //getting the index of the new worldSprite

	worldSprite tempWS;
	tempWS.file = (filename);
	tempWS.imgID = agk::LoadImage(filename.data());
	tempWS.SID = agk::CreateSprite(tempWS.imgID);

	//Hiding the sprite for now
	agk::SetSpriteVisible(tempWS.SID, 0);

	//everything has been loaded, add it to the vector and return the ID
	wS->push_back(tempWS);
	return ID;
}

World::Entry* World::findEntry(std::string name)
{
	int slot = 0;

	for(unsigned int i = 0; i < entry->size(); i++)
	{
		if(name.compare(entry->at(i).name) == 0)
		{
			slot = i;
		}
	}

	return &entry->at(slot);
}

void World::setOvercast(float overcast)
{
	this->overcast = overcast;
}
void World::setTime(float time)
{
	this->time = time;
}
void World::setCloudLayers(int cloudLayers)
{
	this->cloudLayers = cloudLayers;
}
void World::setLastActive(int lastActive)
{
	this->lastActive = lastActive;
}
void World::setIndoors(bool isIndoors)
{
	this->isIndoors = isIndoors;
}

Part* World::getPartFromName(std::string name) //This function goes thru all the parts and looks for one with the name specified //It will return the first part with the name
{
	Part* fPart = 0; //Will be 0 or null if no part is found

	for(unsigned int i = 0; i < part->size(); i++)
	{
		if(part->at(i).getName().compare(name) == 0) //Checking if the names are the same
		{
			fPart = &part->at(i);
			return fPart;
		}
	}

	return fPart;
}
Part* World::getPartFromID(int partID)
{
	if((unsigned) partID >= 0 && (unsigned)partID < part->size())
	{
		return &part->at(partID);
	}
	
	return NULL;
}

int World::getPartAmount()
{
	return part->size();
}
int World::getPartSID(int partID)
{
	return part->at(partID).getSID();
}
int World::getPartPhysState(int partID)
{
	return part->at(partID).getPhysState();
}
int World::getPartUsable(int partID)
{
	return part->at(partID).getUsable();
}
float World::getPartX(int partID)
{
	return part->at(partID).getX();
}
float World::getPartY(int partID)
{
	return part->at(partID).getY();
}
int World::getSkyID()
{
	return skyID;
}
float World::getTime()
{
	return time;
}
float World::getLeftEdge()
{
	float edge = 10000;
	for(unsigned int i = 0; i < part->size(); i++) //Going through all of the parts
	{
		if(part->at(i).getX() - part->at(i).getEdgeRadius() < edge) //Checking if this one is "lefter" than the others
		{
			edge = part->at(i).getX() - part->at(i).getEdgeRadius(); //Updating the edge
		}
	}
	return edge;
}
float World::getWidth()
{
	//Extreme values to make sure something is found
	float max = -100000;
	float min = 100000;

	for(unsigned int i = 0; i < part->size(); i++) //Checking all parts
	{
		float xPos = part->at(i).getX();

		//Checking if the values should be changed
		if(xPos - part->at(i).getEdgeRadius() < min)
		{
			min = xPos - part->at(i).getEdgeRadius();
		}
		if(xPos + part->at(i).getEdgeRadius() > max)
		{
			max = xPos + part->at(i).getEdgeRadius();
		}
	}

	//Calculating the width
	float width = max - min;
	return width;
}
float World::getBottomEdge()
{
	float max = -10000;
	
	for(unsigned int i = 0; i < part->size(); i++)
	{
		if(part->at(i).getY() + part->at(i).getEdgeRadius() > max)
		{
			max = part->at(i).getY() + part->at(i).getEdgeRadius();
		}
	}

	return max;
}
float World::getHeight()
{
	float max = -10000;
	float min = 10000;

	for(unsigned int i = 0; i < part->size(); i++)
	{
		float yPos = part->at(i).getY();
		
		if(yPos - part->at(i).getEdgeRadius() < min)
		{
			min = yPos - part->at(i).getEdgeRadius();
		}
		if(yPos + part->at(i).getEdgeRadius() > max)
		{
			max = yPos + part->at(i).getEdgeRadius();
		}
	}

	float height = max - min;

	return height;
}
int World::getLastActive()
{
	return lastActive;
}

bool World::isLadder(float x, float y)
{
	bool result = false;
	for(unsigned int i = 0; i < part->size(); i++)
	{
		if(part->at(i).getLadder() == 1) //If the part is a ladder
		{
			//Checking for collision with the part
			if(part->at(i).getHit(x, y))
			{
				result = true;

				break; //Exit the loop since nothing matters anymore
			}
		}
	}

	return result;
}
bool World::isGround(float x, float y)
{
	bool isGround = false;
	for(unsigned int i = 0; i < part->size(); i++)
	{
		int physState = part->at(i).getPhysState();
		if(physState == 2 || physState == 1) //Checking if physics is enabled for the sprite
		{
			if(part->at(i).getHit(x, y))
			{
				isGround = true;
			}
		}
	}
	return isGround;
}
int World::getPhysicsCollision(int sensor)
{
	int result = -1;
	for(unsigned int i = 0; i < part->size(); i++)
	{
		if(part->at(i).getPhysState() != 0)
		{
			if(agk::GetPhysicsCollision(sensor, part->at(i).getSID()))
			{
				result = i;
			}
		}
	}

	return result;
}

std::string World::getName()
{
	return name;
}

void World::loadBaseMedia() //Loads media that is used in all worlds
{
	//Creating a vector of the cloud sprites
	cloudBase = new std::vector< CloudBase >;

	//Loading the media
	CloudBase tempCB;
	tempCB.img = agk::LoadImage(GF::getPath("Background/cloud1.png").data());
	tempCB.SID = agk::CreateSprite(tempCB.img);
	agk::SetSpriteDepth(tempCB.SID, 900);
	agk::SetSpriteScale(tempCB.SID, 0.07f, 0.07f);
	agk::SetSpriteVisible(tempCB.SID, 0);

	//Adding the new cloud to the vector
	cloudBase->push_back(tempCB);

	//Creating the next cloud
	tempCB.img = agk::LoadImage(GF::getPath("Background/cloud2.png").data());
	tempCB.SID = agk::CreateSprite(tempCB.img);
	agk::SetSpriteDepth(tempCB.SID, 900);
	agk::SetSpriteScale(tempCB.SID, 0.07f, 0.07f);
	agk::SetSpriteVisible(tempCB.SID, 0);

	//Adding the new cloud to the vector
	cloudBase->push_back(tempCB);

}

void World::setLightModeOn()
{
	//Hiding the sky
	agk::SetSpriteVisible(skyID, 0);
	agk::SetSpriteVisible(lightSky, 1);

	//Hiding the clouds
	for(unsigned int i = 0; i < clouds->size(); i++)
	{
		agk::SetSpriteVisible(clouds->at(i).SID, 0);
	}

	//Setting the color of the rest of the world
	for(unsigned int i = 0; i < part->size(); i++)
	{
		agk::SetSpriteColor(part->at(i).getSID(), 0, 0, 0, 255);
	}
}
void World::setLightModeOff()
{
	//Hiding the sky
	agk::SetSpriteVisible(skyID, 1);
	agk::SetSpriteVisible(lightSky, 0);

	//Hiding the clouds
	for(unsigned int i = 0; i < clouds->size(); i++)
	{
		agk::SetSpriteVisible(clouds->at(i).SID, 1);
	}

	//Setting the color of the rest of the world
	for(unsigned int i = 0; i < part->size(); i++)
	{
		agk::SetSpriteColor(part->at(i).getSID(), 255, 255, 255, 255);
	}
}

float World::paralaxOffset(int depth)
{
	//at depth 1000 the sprite will not move at all, at depth 100 the sprite will not have any paralax offset
	float result;

	float paralaxDepth = depth - 100.0f; //Calculating the depth if 0 is 100

	result = paralaxDepth / 900.0f;

	return result;
}

void World::addPartToUpdate( Part* part)
{
	//Checking if the part is already in the update list
	bool alreadyInList = false;
	std::list< Part* >::iterator it;
	for(it = partsToUpdate->begin(); it != partsToUpdate->end(); it++)
	{
		if(*it == part) alreadyInList = true;
	}

	if(alreadyInList == false) //If the part wasnt in the list already
	{
		partsToUpdate->push_back(part);
	}
}

PathNode* World::findNodeById(int ID)
{
	for(unsigned int i = 0; i < nodes->size(); i++)
	{
		if(nodes->at(i).getVecID() == ID)
		{
			return &nodes->at(i);
		}
	}

	return NULL;
}
PathLink* World::findLinkById(int ID)
{
	for(unsigned int i = 0; i < links->size(); i++)
	{
		if(links->at(i).getID() == ID)
		{
			return &links->at(i);
		}
	}
	return NULL;
}
void World::displayNodes()
{
	for(unsigned int i = 0; i < links->size(); i++)
	{
		//Getting the nodes connected to the link
		PathNode* node0 = findNodeById(links->at(i).getNodeID(0));
		PathNode* node1 = findNodeById(links->at(i).getNodeID(1));

		if(node0 != NULL && node1 != NULL)
		{
			float x1 = agk::WorldToScreenX(node0->getX());
			float x2 = agk::WorldToScreenX(node1->getX());
			float y1 = agk::WorldToScreenY(node0->getY());
			float y2 = agk::WorldToScreenY(node1->getY());

			int r = 255;
			int g = 0;
			int b = 0;

			if(links->at(i).getType() == 1)
			{
				r = 0;
				g = 255;
				b = 0;
			}
			agk::DrawLine(x1, y1, x2, y2, r, g ,b );
		}
	}
}
PathLink* World::findClosestLink(float x, float y)
{
	float delta = 2;
	//Going thru all of the nodes
	float lowestDist = 10000000;
	PathLink* result = NULL;

	for(unsigned int i = 0; i < links->size(); i++)
	{
		PathNode* node0 = findNodeById(links->at(i).getNodeID(0));
		PathNode* node1 = findNodeById(links->at(i).getNodeID(1));

		//Finding the lowset X position
		float beginX = node0->getX();
		float endX = node1->getX();
		float beginY = node0->getY();
		float endY = node1->getY();

		//Making sure begin < end
		if(endX < beginX)
		{
			float buff = endX;
			endX = beginX;
			beginX = buff;
		}
		if(endY < beginY)
		{
			float buff = endY;
			endY = beginY;
			beginY = buff;
		}

		float distX = endX - beginX;
		float distY = endY - beginY;
		float distRatio = distY / distX;

		for(float pX = beginX; pX < endX; pX += 2.0f)
		{
			float xPos = pX - beginX;
			float yPos = xPos * distRatio;

			//Calculating the position of the point on the line
			float pY = beginY + yPos;
			
			//Calculating the distance of the point from the 'target' point
			float pDistX = pX - x;
			float pDistY = pY - y;
			float pDist = sqrt(pow(pDistX, 2) + pow(pDistY, 2));

			//This point is closer
			if(pDist < lowestDist)
			{
				lowestDist = pDist;

				result = &links->at(i);
			}
		}
	}

	return result;
}
std::vector<PathLink*>* World::getPath(float startX, float startY, float endX, float endY)
{
	int nextID = 0;
	struct ListNode
	{
		int ID;

		PathLink* link;
		int parent;
	};

	std::vector<PathLink*>* result = new std::vector<PathLink*>;
	//Finding the start and end link
	ListNode startLink;
	startLink.link = findClosestLink(startX, startY);
	startLink.parent = -1;
	ListNode endLink;
	endLink.link= findClosestLink(endX, endY);

	std::deque< ListNode > openList;
	std::deque< ListNode > closedList;
	openList.push_back(startLink);

	while(openList.size() > 0)
	{
		//Getting the nodes connected by the link
		PathNode* nodes[2];
		nodes[0] = findNodeById(openList.at(0).link->getNodeID(0));
		nodes[1] = findNodeById(openList.at(0).link->getNodeID(1));

		for(unsigned int i = 0; i < 2; i++)
		{
			std::vector<int>* linkID = nodes[i]->getLinks();

			for(unsigned int l = 0; l < linkID->size(); l++)
			{
				//Checking if the links are already on the open list
				bool onList = false;
				for(unsigned int o = 0; o < openList.size(); o++)
				{
					if(openList.at(o).link->getID() == linkID->at(l))
					{
						onList = true;
					}
				}
				for(unsigned int c = 0; c < closedList.size(); c++)
				{
					if(closedList.at(c).link->getID() == linkID->at(l))
					{
						onList = true;
					}
				}

				if(onList == false) //It wasn't on the list already
				{
					ListNode tempNode;
					tempNode.link = findLinkById(linkID->at(l));
					tempNode.parent = openList.at(0).ID;
					tempNode.ID = nextID;
					openList.push_back(tempNode);
					//openList.push_back(findLinkById(linkID->at(l)));
					nextID++;
				}
			}
		}
		//Adding the link to the closed list
		closedList.push_back(openList.at(0));
		openList.pop_front();

		//Checking if the target has been found
		if(closedList.back().link->getID() == endLink.link->getID())
		{
			break;
		}
	}
	//If the goal wasn't found
	if(closedList.back().link->getID() != endLink.link->getID())
	{
		delete result; //Cleaning up
		return NULL;
	}
	
	//Backtracking the path to find the start and create a path
	bool pathSaved = false; //True if the path has been found
	bool pathSaving = true;//True while the path is being saved
	//Adding the end node to the path
	std::vector<ListNode> resultPath;
	resultPath.push_back(closedList.back());
	while(pathSaving == true)
	{
		//Getting the parent of the current node
		int parentID = resultPath.back().parent;

		if(parentID == -1) //The first node has been found
		{
			pathSaved = true;
			break;
		}

		//Finding the link with the ID
		bool foundParent = false;;
		for(unsigned int i = 0; i < closedList.size(); i++)
		{
			if(closedList.at(i).ID == parentID)
			{
				//Add the link to the path
				resultPath.push_back(closedList.at(i));
					
				foundParent = true;
				//Exit the loop
				break;
			}
		}

		if(foundParent == false)
		{
			pathSaving = false;
			pathSaved = false;
		}
	}

	if(pathSaved == false)
	{
		return NULL;
	}

	//The path has been tracked, save it
	for(unsigned int i = 0; i < resultPath.size(); i++)
	{
		result->push_back(resultPath.at(i).link);
	}

	return result;
}

/////////////////////////////////////////////////////////
//						Nodes
/////////////////////////////////////////////////////////
void PathNode::create(int vecID, float x, float y)
{
	this->x = x;
	this->y = y;
	this->vecID = vecID;
}

void PathNode::addLink(int ID)
{
	this->links.push_back(ID);
}

int PathNode::getVecID()
{
	return vecID;
}
float PathNode::getX()
{
	return x;
}
float PathNode::getY()
{
	return y;
}
std::vector<int>* PathNode::getLinks()
{
	return &links;
}

/////////////////////////////////////////////////////////
//						Links
/////////////////////////////////////////////////////////
void PathLink::create(int vecID, int node0, int node1, int type)
{
	this->vecID = vecID;
	nodes[0] = node0;
	nodes[1] = node1;
	this->type = type;
}

int PathLink::getNodeID(int index)
{
	return nodes[index];
}

int PathLink::getType()
{
	return type;
}
int PathLink::getID()
{
	return vecID;
}