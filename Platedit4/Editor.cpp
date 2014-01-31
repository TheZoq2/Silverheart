#include "Editor.h"


Editor::Editor(void)
{
	
}


Editor::~Editor(void)
{
}

void Editor::setup()
{
	parts = new std::vector< Part >;
	selParts = new std::vector< int >;

	nodes = new std::vector< Node >;
	links = new std::vector< Link >;

	cameraX = (float)agk::GetVirtualWidth();
	cameraY = (float)agk::GetVirtualWidth();
	cameraZoom = 1;

	selNode = -1;
}

void Editor::update(int selTool, bool uiActive)
{
	float sMoveX = i_sMoveX;
	float sMoveY = i_sMoveY;
	float moveX = i_moveX;
	float moveY = i_moveY;
	float mouseX = i_mx;
	float mouseY = i_my;
	
	float scrollAmount = Input::scrollAmount();

	if(Input::mouseClick()) //If the mouse was just clicked
	{
		//Preventing movement from happening when the user was not holding the mouse
		moveX = 0;
		moveY = 0;
	}

	if(uiActive == false)
	{
		if(Input::mouseClick())
		{
			//Selecting new parts
			for(unsigned int i = 0; i < parts->size(); i++)
			{
				//Checking for collision
				if(parts->at(i).isHit(mouseX, mouseY))
				{
					if(Input::selectMultiple() == false)
					{
						selParts->clear();
					}

					//checking if the part is already selected
					bool alreadySelected = false;
					for(unsigned int n = 0; n < selParts->size(); n++)
					{
						if(selParts->at(n) == parts->at(i).getVecID())
						{
							alreadySelected = true;
						}
					}
					if(alreadySelected == false)
					{
						selParts->push_back(parts->at(i).getVecID());
					}
				}
			}

			if(selTool == tool_place)
			{
				if(agk::GetFileExists(selImage.data()))
				{
					Part part;
					part.create(nextPart, selImage, mouseX, mouseY, e_defaultDepth);

					parts->push_back(part);

					nextPart++;
				}
			}

			if(selTool == tool_createNode) //Creating nodes
			{
				Node tempNode;
				tempNode.create(nextNode, mouseX, mouseY);
				nodes->push_back(tempNode);

				//Checking if a node is already selected
				if(selNode != -1)
				{
					Link tempLink;
					tempLink.create(nextLink, nextNode, selNode, 0);

					links->push_back(tempLink);
				}

				selNode = nextNode; //Selecting the new node to make chains easier to make

				nextNode++;
			}
			if(selTool == TOOL_MOVENODE || selTool == TOOL_JUMPLINK || selTool == TOOL_LINK) //Selecting nodes
			{
				lastSelNode = selNode;
				selNode = -1;
				for(unsigned int i = 0; i < nodes->size(); i++)
				{
					if(nodes->at(i).getHit(mouseX, mouseY))
					{
						selNode = nodes->at(i).getVecID();

						//If a link should be created
						if(selTool == TOOL_JUMPLINK)
						{
							if(selNode != lastSelNode && findNodeByID(lastSelNode) != NULL && findNodeByID(selNode) != NULL)
							{
								Link tempLink;
								tempLink.create(nextLink, selNode, lastSelNode, 1);

								links->push_back(tempLink);
								nextLink++;
							}
						}

						if(selTool == TOOL_LINK)
						{
							if(selNode != lastSelNode && findNodeByID(lastSelNode) != NULL && findNodeByID(selNode) != NULL)
							{
								Link tempLink;
								tempLink.create(nextLink, selNode, lastSelNode, 0);

								links->push_back(tempLink);
								nextLink++;
							}
						}
					}
				}
			}
		}
	}
	if(Input::mouseHold())
	{
		if(selTool == tool_move)
		{
			//Going through all the selected parts
			for(unsigned int i = 0; i < selParts->size(); i++)
			{
				Part* part = this->findPartByID(selParts->at(i));
				
				if(part != NULL)
				{
					part->move(moveX, moveY);
				}
			}
		}
		if(selTool == tool_scale)
		{
			Part* part0;
			if(selParts->size() > 0)
			{
				part0 = findPartByID(selParts->at(0));
			}
			//Going thru all the selected parts
			for(unsigned int i = 0; i < selParts->size(); i++)
			{
				Part* part = this->findPartByID(selParts->at(i));
				
				if(part != NULL)
				{
					float xScale = moveX / 500.0f;
					float yScale = moveX / 500.0f;
					float xScaleChange = 1 - xScale;
					float yScaleChange = 1 - yScale;
					part->scale(xScaleChange, xScaleChange);

					//Moving parts towards the center
					if(part0 != NULL)
					{
						float xDist = part->getX() - part0->getX();
						float yDist = part->getY() - part0->getY();

						float newXDist = xDist * xScaleChange;
						float newYDist = yDist * yScaleChange;

						float newX = part0->getX() + newXDist;
						float newY = part0->getY() + newYDist;

						part->setPosition(newX, newY);
					}
				}
			}
		}
		if(selTool == tool_rotate)
		{
			float lowestX = 1000000;
			float lowestY = 1000000;
			float highestX = - 100000;
			float highestY = - 100000;
			float xCenter = 0;
			float yCenter = 0;
			for(unsigned int i = 0; i < selParts->size(); i++)
			{
				Part* part = findPartByID(selParts->at(i));
				if(part != NULL)
				{
					if(part->getX() < lowestX) lowestX = part->getX();
					if(part->getY() < lowestY) lowestY = part->getY();
					if(part->getX() > highestX) highestX = part->getX();
					if(part->getX() > highestY) highestY = part->getY();
				}
			}

			//Calculating the center point of the parts
			xCenter = lowestX + (highestX - lowestX) / 2.0f;
			yCenter = lowestY + (highestY - lowestY) / 2.0f;

			float xDiff = mouseX - xCenter;
			float yDiff = mouseY - yCenter;
			float angle = agk::ATanFull(xDiff, yDiff);

			//Rotating those parts
			for(unsigned int i = 0; i < selParts->size(); i++)
			{
				Part* part = findPartByID(selParts->at(i));
				if(part != NULL)
				{
					float angleDiff = angle - part->getAngle();

					part->setAngle(angle);

					//Positioning the sprites based on the rotation and offset from the center
					float offsetX = part->getX() - xCenter;
					float offsetY = part->getY() - yCenter;
					float offsetDist = sqrt(pow(offsetX, 2) + pow(offsetY, 2));

					float offsetAngle = agk::ATanFull(offsetX, offsetY);
					agk::Print(offsetAngle);

					float newX = xCenter + agk::Cos(offsetAngle - 90 + angleDiff) * offsetDist;
					float newY = yCenter + agk::Sin(offsetAngle - 90 + angleDiff) * offsetDist;
				}
			}
		}

		if(selTool == TOOL_MOVENODE)
		{
			Node* node = findNodeByID(selNode);
			
			if(node != NULL)
			{
				node->move(i_moveX, i_moveY);
			}
		}
	}

	//Highlighting the selected parts
	for(unsigned int i = 0; i < selParts->size(); i++)
	{
		Part* part = findPartByID(selParts->at(i));

		if(part != NULL)
		{
			part->highlight(255, 255, 255);
		}
	}

	//Highlighting nodes
	if(selTool == tool_createNode || selTool == TOOL_MOVENODE || selTool == TOOL_JUMPLINK || selTool == TOOL_LINK)
	{
		if(Input::cameraMove())
		{
			selNode = -1;
		}

		for(unsigned int i = 0; i < nodes->size(); i++)
		{
			nodes->at(i).setVisible(1);
			nodes->at(i).setColor(255, 0, 0, 150);
		}

		if(selNode != -1)
		{
			Node* node = findNodeByID(selNode);
			if(node != NULL)
			{
				node->setColor(0, 255, 0, 150);
			}
		}

		//Drawing the links
		for(unsigned int i = 0; i < links->size(); i++)
		{
			//Getting the nodes
			Node* node0 = findNodeByID(links->at(i).getNode(0));
			Node* node1 = findNodeByID(links->at(i).getNode(1));

			if(node0 != NULL && node1 != NULL)
			{
				//Drawing a line between the nodes
				float x1 = agk::WorldToScreenX(node0->getX());
				float x2 = agk::WorldToScreenX(node1->getX());
				float y1 = agk::WorldToScreenY(node0->getY());
				float y2 = agk::WorldToScreenY(node1->getY());
				
				if(links->at(i).getType() == 0)
				{
					agk::DrawLine(x1, y1, x2, y2, 255, 0, 0);
				}
				else if(links->at(i).getType() == 1)
				{
					agk::DrawLine(x1, y1, x2, y2, 0, 255, 0);
				}
			}
		}
	}
	else
	{
		for(unsigned int i = 0; i < nodes->size(); i++)
		{
			nodes->at(i).setVisible(0);
		}
	}

	//Camera movement
	if(Input::cameraMove())
	{
		cameraX = cameraX + -sMoveX / cameraZoom;
		cameraY = cameraY + -sMoveY / cameraZoom;
	}
	cameraZoom = cameraZoom + 0.01f * scrollAmount;

	agk::SetViewZoom(cameraZoom);
	agk::SetViewOffset(cameraX - agk::GetVirtualWidth() / 2, cameraY - agk::GetVirtualHeight() / 2);
}
void Editor::updateSelectedWindow( UI* uiGroup)
{
	if(selParts->size() == 0) //If no parts are selected
	{
		if(uiGroup->getWindowExists("selected"))
		{
			uiGroup->removeWindow("selected");
		}
	}
	else
	{
		if(selParts->size() == 1)
		{
			if(lastSelID != selParts->at(0))
			{
				uiGroup->removeWindow("selected");
			}

			lastSelID = selParts->at(0);
			Part* part = findPartByID(selParts->at(0));

			if(uiGroup->getWindowExists("selected") == 0)
			{
				uiGroup->addWindow("selected", "1x1.png", (float)agk::GetVirtualWidth() - 250.0f, 0, 250.0f, (float)agk::GetVirtualHeight());
				uiGroup->setWindowColor("selected", 150, 150, 150, 255);

				uiGroup->addEditboxToWindow("selected", "name", 5, 5, 250, 16);
				uiGroup->setEditboxValue("selected", "name", part->getData());

				uiGroup->addSimpleListToWindow("selected", "dataList", 5, 25, 240, 200, "Data");
			}

			//Adding the data to the data list
			uiGroup->clearList("selected", "dataList");
			std::string listVal = "Depth: ";
			char* p = agk::Str(part->getDepth());
			listVal.append(p);
			delete p;
			uiGroup->addToSimpleList("selected", "dataList", listVal);
			p = agk::Str(part->getPhysState());
			listVal = "Phys state: ";
			listVal.append(p);
			delete p;
			uiGroup->addToSimpleList("selected", "dataList", listVal);


			part->setData(uiGroup->getEditboxValue("selected", "name"));

			
		}
		else //If there is more than 1 part selected
		{
			if(uiGroup->getWindowExists("selected"))
			{
				uiGroup->removeWindow("selected");
			}
		}
	}
}

void Editor::incPhysState(int amount)
{
	for(unsigned int i = 0; i < selParts->size(); i++)
	{
		Part* part = findPartByID(selParts->at(i));

		part->incPhysState(amount);
	}
}
void Editor::incDepth(int amount)
{
	for(unsigned int i = 0; i < selParts->size(); i++)
	{
		Part* part = findPartByID(selParts->at(i));

		part->incDepth(amount);
	}
}

void Editor::setSelImage(std::string selImage)
{
	this->selImage = selImage;
}

Part* Editor::findPartByID(int ID)
{
	for(unsigned int i = 0; i < parts->size(); i++)
	{
		if(parts->at(i).getVecID() == ID)
		{
			return &parts->at(i);
		}
	}

	return NULL;
}
void Editor::removePartWithID(int ID)
{
	std::vector< Part >::iterator it;
	for(it = parts->begin(); it != parts->end(); it++)
	{
		if(it->getVecID() == ID)
		{
			it->remove();
			parts->erase(it);

			return; //Exiting the function
		}
	}
}

void Editor::saveMap(std::string filename)
{
	//Creating the file
	int fileID = agk::OpenToWrite(filename.data());
		//Writing the filename
		agk::WriteString(fileID, "1"); //Version
		//Writing the amount of data
		agk::WriteString(fileID, "0"); //Special data

		//Writing the actual level data
		std::string partAmount = agk::Str((signed)parts->size());
		agk::WriteString(fileID, partAmount.data());
		for(unsigned int i = 0; i < parts->size(); i++)
		{
			std::string imgName = parts->at(i).getImgName();
			float xPos = parts->at(i).getX();
			float yPos = parts->at(i).getY();
			float scaleX = parts->at(i).getScaleX();
			float scaleY = parts->at(i).getScaleY();
			float angle = parts->at(i).getAngle();
			int physState = parts->at(i).getPhysState();
			int depth = parts->at(i).getDepth();

			//Starting a new part
			agk::WriteString(fileID, "|");
			
			std::string partString = DataEditor::createData("imgName", imgName);
			partString.append(";");
			partString.append(DataEditor::createData("xPos", xPos));
			partString.append(";");
			partString.append(DataEditor::createData("yPos", yPos));
			partString.append(";");
			partString.append(DataEditor::createData("scaleX", scaleX));
			partString.append(";");
			partString.append(DataEditor::createData("scaleY", scaleY));
			partString.append(";");
			partString.append(DataEditor::createData("angle", angle));
			partString.append(";");
			partString.append(DataEditor::createData("physState", physState));
			partString.append(";");
			partString.append(DataEditor::createData("depth", depth));
			partString.append(";");
			partString.append(parts->at(i).getData());

			//Writing the data
			agk::WriteString(fileID, partString.data());
		}

		//agk::WriteString(fileID, "2"); //The amount of extra data
	
		//Writing the node and link data
		std::string nodeData = "nodes={";
		for(unsigned int i = 0; i < nodes->size(); i++)
		{
			nodeData.append("[");
			nodeData.append(DataEditor::createData("vecID", nodes->at(i).getVecID()));
			nodeData.append(",");
			nodeData.append(DataEditor::createData("x", nodes->at(i).getX()));
			nodeData.append(",");
			nodeData.append(DataEditor::createData("y", nodes->at(i).getY()));
			nodeData.append("]");
		}
		nodeData.append("};");

		agk::WriteString(fileID, nodeData.data());
		//link data
		std::string linkData = "links={";
		for(unsigned int i = 0; i < links->size(); i++)
		{
			linkData.append("[");
			linkData.append(DataEditor::createData("vecID", links->at(i).getVecID()));
			linkData.append(",");
			linkData.append(DataEditor::createData("node0", links->at(i).getNode(0)));
			linkData.append(",");
			linkData.append(DataEditor::createData("node1", links->at(i).getNode(1)));
			linkData.append(",");
			linkData.append(DataEditor::createData("type", links->at(i).getType()));
			linkData.append("]");
		}
		linkData.append("};");
		
		agk::WriteString(fileID, linkData.data());
		
	agk::CloseFile(fileID);
}

void Editor::loadMap(std::string filename)
{
	//Checking if the file exists
	if(agk::GetFileExists(filename.data()))
	{
		//Starting to read the file
		int fileID = agk::OpenToRead(filename.data());

		//Pointer for removing the data once it has been read
		char* p;
		p = agk::ReadString(fileID); //Version
		delete[] p;

		p = agk::ReadString(fileID); //Special data
		delete[] p;

		p = agk::ReadString(fileID); //Part amount
		int partAmount = agk::Val(p);
		delete[] p;
		for(int i = 0; i < partAmount; i++) //Reading all the parts
		{
			//Default values
			std::string imgName = "ERROR:NoValue";
			float xPos;
			float yPos;
			float scaleX = 1;
			float scaleY = 0;
			float angle = 0;
			int depth = 100;
			int physState = 1;

			std::string additionalData;

			p = agk::ReadString(fileID); // '|'
			delete[] p;

			std::string partData = agk::ReadString(fileID);

			//Handleing the part data
			std::vector< std::string > dataSegments = zString::split(partData, ";");

			for(unsigned int i = 0; i < dataSegments.size(); i++)
			{
				//Splitting the data into type and value
				std::vector< std::string > dataParts = zString::split(dataSegments.at(i), ":");

				//Making sure the data is correct
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
					else
					{
						additionalData.append(dataType);
						additionalData.append(":");
						additionalData.append(dataValue);
						additionalData.append(";");
					}
				}
			}

			//All the data has been loaded from the file
			//Making sure important paramaters were set
			if(imgName.compare("ERROR:NoValue") != 0)
			{
				Part tempPart; //Creating the new part
				tempPart.create(nextPart, imgName, xPos, yPos, depth);
				tempPart.setScale(scaleX, scaleY);
				tempPart.setPosition(xPos, yPos);
				tempPart.setAngle(angle);
				tempPart.setPhysState(physState);
				tempPart.setDepth(depth);
				tempPart.setData(additionalData);

				nextPart++;
				parts->push_back(tempPart);
			}
		}

		while(agk::FileEOF(fileID) != 1)
		{
			//Reading the string
			p = agk::ReadString(fileID);
			std::string string = p;
			delete[] p;

			//getting the position of the equal sign
			std::size_t equalPos = string.find("=");

			if(equalPos != std::string::npos) //Making sure there is actual data
			{
				std::string groupType = string.substr(0, equalPos);
				std::string groupData = string.substr(equalPos + 1);
				
				//Checking what kind of data this is
				if(groupType.compare("nodes") == 0) //Node data
				{
					int highestVecID = -1;
					bool groupRead = false;
					std::size_t lastStart = 0;
					std::size_t lastEnd = 0;

					while(groupRead == false)
					{
						//Finding the next [
						std::size_t start = groupData.find("[", lastEnd);
						std::size_t end = groupData.find("]", lastEnd + 1);
						if(start != std::string::npos)
						{
							std::string groupString;
							groupString = groupData.substr(start + 1, end - start - 1);

							readNodeGroup(groupString);

							lastEnd = end; //Updating the last ]
						}
						else
						{
							//No more [ could be found, the search is complete
							groupRead = true;
						}
					}
				}
				else if(groupType.compare("links") == 0)
				{
					int highestVecID = -1;
					bool groupRead = false;
					std::size_t lastStart = 0;
					std::size_t lastEnd = 0;

					while(groupRead == false)
					{
						//Finding the next [
						std::size_t start = groupData.find("[", lastEnd);
						std::size_t end = groupData.find("]", lastEnd + 1);
						if(start != std::string::npos)
						{
							std::string groupString;
							groupString = groupData.substr(start + 1, end - start - 1);

							readLinkGroup(groupString);

							lastEnd = end; //Updating the last ]
						}
						else
						{
							//No more [ could be found, the search is complete
							groupRead = true;
						}
					}
				}
			}
		}

		//Closing the file
		agk::CloseFile(fileID);

		//Updating the nextID for nodes and links
		int highestNodeID = 0;
		for(unsigned int i = 0; i < nodes->size(); i++)
		{
			if(nodes->at(i).getVecID() > highestNodeID) highestNodeID = nodes->at(i).getVecID();
		}
		nextNode = highestNodeID + 1;
		
		int highestlinkID = 0;
		for(unsigned int i = 0; i < links->size(); i++)
		{
			if(links->at(i).getVecID() > highestlinkID) highestlinkID = links->at(i).getVecID();
		}
		nextLink = highestlinkID;
	}
	else
	{
		DebugConsole::addC("Failed to load level: ");DebugConsole::addC(filename.data());
		DebugConsole::addToLog(" -- file did not exist");
	}
}
void Editor::readNodeGroup(std::string group)
{
	int highestVecID = -1;
	std::vector< std::string > data = zString::split(group, ",");
	
	int vecID = 0;
	float xPos = 0;
	float yPos = 0;

	for(unsigned int i = 0; i < data.size(); i++)
	{
		std::vector< std::string > strings = zString::split(data.at(i), ":");

		std::string dataType = strings.at(0);
		std::string dataValue = strings.at(1);

		//VecIDs are recreated when loading
		if(dataType.compare("x") == 0)
		{
			xPos = atof(dataValue.data());
		}
		else if(dataType.compare("y") == 0)
		{
			yPos = atof(dataValue.data());
		}
		else if(dataType.compare("vecID") == 0)
		{
			vecID = atoi(dataValue.data());
		}
	}

	Node tempNode;
	tempNode.create(vecID, xPos, yPos);
	nodes->push_back(tempNode);
}
void Editor::readLinkGroup(std::string group)
{
	int highestVecID = -1;
	std::vector< std::string > data = zString::split(group, ",");
	
	int vecID = 0;
	int node0 = 0;
	int node1 = 0;
	int type = 0;

	for(unsigned int i = 0; i < data.size(); i++)
	{
		std::vector< std::string > strings = zString::split(data.at(i), ":");

		std::string dataType = strings.at(0);
		std::string dataValue = strings.at(1);

		//VecIDs are recreated when loading
		if(dataType.compare("vecID") == 0)
		{
			vecID = atoi(dataValue.data());
		}
		else if(dataType.compare("node0") == 0)
		{
			node0 = atoi(dataValue.data());
		}
		else if(dataType.compare("node1") == 0)
		{
			node1 = atoi(dataValue.data());
		}
		else if(dataType.compare("type") == 0)
		{
			type = atoi(dataValue.data());
		}
	}

	Link tempLink;
	tempLink.create(vecID, node0, node1, type);
	links->push_back(tempLink);
}

void Editor::removeSelected()
{
	for(unsigned int i = 0; i < selParts->size(); i++)
	{
		removePartWithID(selParts->at(i));
	}

	selParts->clear();
}

Node* Editor::findNodeByID(int ID)
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

////////////////////////////////////////////////////////////////////////////////
void Node::create(int vecID, float x, float y)
{
	this->vecID = vecID;
	this->x = x;
	this->y = y;

	SID = agk::CloneSprite(1);
	agk::SetSpriteScale(SID, 10, 10);
	agk::SetSpritePositionByOffset(SID, x, y);
}

void Node::move(float xAmount, float yAmount)
{
	float newX = x + xAmount;
	float newY = y + yAmount;

	setPositon(newX, newY);
}

void Node::setVisible(int visible)
{
	agk::SetSpriteVisible(SID, visible);
}
void Node::setColor(int r, int g, int b, int a)
{
	agk::SetSpriteColor(SID, r, g, b, a);
}
void Node::setPositon(float x, float y)
{
	this->x = x;
	this->y = y;
	agk::SetSpritePositionByOffset(SID, x, y);
}

bool Node::getHit(float x, float y)
{
	if(agk::GetSpriteHitTest(SID, x, y))
	{
		return true;
	}
	return false;
}
int Node::getVecID()
{
	return this->vecID;
}
float Node::getX()
{
	return x;
}
float Node::getY()
{
	return y;
}

void Link::create(int vecID, int node1, int node2, int type)
{
	this->vecID = vecID;
	this->type = type;

	nodes[0] = node1;
	nodes[1] = node2;
}
int Link::getNode(int index)
{
	if(index >= 0 && index < 2)
	{
		return nodes[index];
	}

	return -1;
}
int Link::getType()
{
	return type;
}
int Link::getVecID()
{
	return vecID;
}