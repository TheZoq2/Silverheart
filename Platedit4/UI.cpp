#include "UI.h"



UI::UI(void)
{
}
UI::~UI(void)
{
}

void UI::setup()
{
	windows = new std::vector< Window >;
}

void UI::update()
{
	std::vector< Window >::iterator windowIt;
	std::vector< Window >::iterator hitIt;

	bool windowHit = false;
	windowHitAmount = 0;
	for(windowIt = windows->begin(); windowIt != windows->end(); windowIt++) //Going thru all the windows
	{
		if(windowIt->update(i_mx, i_my))
		{
			//The mouse is on this window
			hitIt = windowIt;

			windowHit = true;

			windowHitAmount++;
		}
	}

	if(windowHit == true) //If a window was hit
	{
		hitIt->updateInput(i_mx, i_my);
	}
}
void UI::updateScissors()
{
	for(unsigned int i = 0; i < windows->size(); i++)
	{
		windows->at(i).updateScissors();
	}
}

bool UI::getUIActive()
{
	if(windowHitAmount == 0)
	{
		return false;
	}
	else return true;
}

void UI::addWindow(std::string ID, std::string bgName, float x, float y, float sizeX, float sizeY)
{
	Window tempWindow;
	tempWindow.create(ID, bgName, x, y, sizeX, sizeY);

	windows->push_back(tempWindow);
}
void UI::removeWindow(std::string windowID)
{
	//Finding the window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->remove();

		//Removing the window from the vector
		this->deleteWindowByID(windowID);
	}
	else
	{
		DebugConsole::addC("Failed to remove window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addC(" -- Window did not exist");
	}
}

void UI::addListToWindow(std::string window, std::string listID, float offsetX, float offsetY, float width, float height, std::vector< float >* colWidth, std::vector< std::string >* colHeaders)
{
	//Adding the list
	getWindowByID(window)->addList(listID, offsetX, offsetY, width, height, colWidth, colHeaders);
}
void UI::addSimpleListToWindow(std::string windowID, std::string listID, float x, float y, float width, float height, std::string header)
{
	//Finding the window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->addSimpleList(listID, x, y, width, height, header);
	}
	else
	{
		DebugConsole::addC("Failed to add simple list: ");DebugConsole::addC(listID.data());
		DebugConsole::addC(" to window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addC(" -- Window did not exist");
	}
}
void UI::removeList(std::string windowID, std::string listID)
{
	//Finding the window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->removeList(listID);
	}
	else
	{
		DebugConsole::addC("Failed to remove list, window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" did not exist");
	}
}
void UI::clearList(std::string windowID, std::string listID)
{
	//Finding the window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->clearList(listID);
	}
	else
	{
		DebugConsole::addC("Failed to clear list, window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" did not exist");
	}
}
void UI::addToList(std::string windowID, std::string listID, std::vector< std::string >* values)
{
	//Finding the window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->addToList(listID, values);
	}
	else
	{
		DebugConsole::addC("Failed to add item to list, window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" did not exist");
	}
}
void UI::addToSimpleList(std::string windowID, std::string listID, std::string value)
{
	Window* window = getWindowByID(windowID);
	if(window != NULL)
	{
		window->addToSimpleList(listID, value);
	}
	else
	{
		DebugConsole::addC("Failed to add item to simple list, window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" did not exist");
	}
};

void UI::addImgListToWindow(std::string windowID, std::string vecID, int cloneID, float x, float y, float width, float height, float imgWidth, float imgHeight, int r, int g, int b, int a)
{
	//Finding the right window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->addImgList(vecID, cloneID, x, y, width, height, imgWidth, imgHeight, r, g, b, a);
	}
	else
	{
		DebugConsole::addC("Failed to add imgList: ");DebugConsole::addC(vecID.data());
		DebugConsole::addC(" to window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" -- window did not exist");
	}
}
void UI::addImageToImgList(std::string windowID, std::string vecID, std::string img, std::string value)
{
	Window* window = getWindowByID(windowID);
	if(window != NULL)
	{
		window->addImageToImgList(vecID, img, value);
	}
	else
	{
		DebugConsole::addC("Failed to add image to imglist, window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" did not exist");
	}
}
std::string UI::getImgListSelValue(std::string windowID, std::string vecID)
{
	Window* window = getWindowByID(windowID);
	if(window != NULL)
	{
		return window->getImgListSelValue(vecID);
	}
	else
	{
		DebugConsole::addC("Failed to get selected value from imglist, window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" did not exist");
	}

	return "";
}

void UI::addEditboxToWindow(std::string windowID, std::string vecID, float x, float y, float sizeX, float sizeY)
{
	//Finding the right window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->addEditbox(vecID, x, y, sizeX, sizeY);
	}
	else
	{
		DebugConsole::addC("Failed to add editbox: ");DebugConsole::addC(vecID.data());
		DebugConsole::addC(" to window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" -- window did not exist");
	}
}
std::string UI::getEditboxValue(std::string windowID, std::string vecID)
{
	Window* window = this->getWindowByID(windowID);

	if(window != NULL)
	{
		return window->getEditboxValue(vecID);
	}
	else
	{
		DebugConsole::addC("Failed to get editbox: ");DebugConsole::addC(vecID.data());DebugConsole::addC(" value -- Window did not exist: ");
		DebugConsole::addToLog(windowID.data());
	}
	return "";
}
void UI::setEditboxValue(std::string windowID, std::string vecID, std::string value)
{
	Window* window = this->getWindowByID(windowID);

	if(window != NULL)
	{
		window->setEditboxValue(vecID, value);
	}
	else
	{
		DebugConsole::addC("Failed to set editbox: ");DebugConsole::addC(vecID.data());DebugConsole::addC(" value -- Window did not exist: ");
		DebugConsole::addToLog(windowID.data());
	}
}

void UI::addColorButtonToWindow(std::string windowID, std::string buttonID, float x, float y, float width, float height, std::string text)
{
	//Finding the right window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->addColorButton(buttonID, x, y, width, height, text);
	}
	else
	{
		DebugConsole::addC("Failed to add button: ");DebugConsole::addC(buttonID.data());
		DebugConsole::addC(" to window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" -- window did not exist");
	}
}
void UI::setButtonColor(std::string windowID, std::string buttonID, int r, int g, int b, int hr, int hg, int hb)
{
	//Finding the right window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->setButtonColor(buttonID, r, g, b, hr, hg, hb);
	}
	else
	{
		DebugConsole::addC("Failed to set button color: ");DebugConsole::addC(buttonID.data());
		DebugConsole::addC(" in window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" -- window did not exist");
	}
}

void UI::setWindowColor(std::string ID, int r, int g, int b, int a)
{
	Window* window = getWindowByID(ID); //Getting a pointer to the window
	
	if(window != NULL)
	{
		window->setBgColor(r, b, g, a);
	}
}

bool UI::getWindowExists(std::string ID)
{
	Window* window = getWindowByID(ID);

	if(window != NULL) //Checking if the window did exist
	{
		return true;
	}

	return false;
}

//Private function
Window* UI::getWindowByID(std::string ID)
{
	std::vector< Window >::iterator it;
	for(it = windows->begin(); it != windows->end(); it++) //Going through all the windows
	{
		if(strcmp(it->getID().data(), ID.data()) == 0) //Checking if the window has the correct ID
		{
			return it._Ptr; //Return the pointer for the window
		}
	}

	return NULL;
}
void UI::deleteWindowByID(std::string ID)
{
	std::vector< Window >::iterator it;
	for(it = windows->begin(); it != windows->end(); it++)
	{
		if(it->getID().compare(ID) == 0)
		{
			windows->erase(it);

			return;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void Window::create(std::string vecID, std::string bgName, float x, float y, float sizeX, float sizeY)
{
	this->vecID = vecID;

	this->bgName = bgName.data();
	this->x = x;
	this->y = y;

	this->sizeX = sizeX;
	this->sizeY = sizeY;

	//Loading/creating the media

	std::string bgPath = GF::getPath(bgName.data());

	if(agk::GetFileExists(bgPath.data()))
	{
		bgImg = agk::LoadImage(bgPath.data());
		bgSID = agk::CreateSprite(bgImg);

		agk::FixSpriteToScreen(bgSID, 1);

		agk::SetSpritePosition(bgSID, x, y);
		agk::SetSpriteScale(bgSID, sizeX / agk::GetImageWidth(bgImg), sizeY / agk::GetImageHeight(bgImg));
	}
	else
	{
		DebugConsole::addC("Failed to create window, background image did not exist: ");DebugConsole::addToLog(bgPath.data());
	}

	nextList = 0;

	//Initialising vectors
	lists = new std::vector< List >;
	buttons = new std::vector< Button >;
	imgLists = new std::vector< ImgList >;
	editboxes = new std::vector< UI_editbox >;
}
int Window::update(float mx, float my)
{
	//Converting the cordinates to world cordinates
	float startX = agk::ScreenToWorldX(x);
	float endX = agk::ScreenToWorldX(x + sizeX);
	float startY = agk::ScreenToWorldY(y);
	float endY = agk::ScreenToWorldY(y + sizeY);

	if(mx > startX && mx < endX && my > startY && my < endY) //If the mouse is on the window
	{
		return true; 
	}
	else
	{
		//Changing the state of buttons to the default value when the window is not hit
		for(unsigned int i = 0; i < buttons->size(); i++)
		{
			buttons->at(i).reset();
		}
	}
	return false;
}
void Window::updateInput(float mx, float my)
{
	//Updating individual UI elements
	for(unsigned int i = 0; i < lists->size(); i++)
	{
		lists->at(i).updateInput(mx, my);
	}
	for(unsigned int i = 0; i < imgLists->size(); i++)
	{
		imgLists->at(i).updateInput(mx, my);
	}

	for(unsigned int i = 0; i < buttons->size(); i++)
	{
		buttons->at(i).updateInput(mx, my);
	}
}
void Window::remove()
{
	if(agk::GetSpriteExists(bgSID))
	{
		agk::DeleteSprite(bgSID);
	}
	for(unsigned int i = 0; i < imgLists->size(); i++)
	{
		imgLists->at(i).remove();
	}
	for(unsigned int i = 0; i < lists->size(); i++)
	{
		lists->at(i).remove();
	}
	for(unsigned int i = 0; i < editboxes->size(); i++)
	{
		editboxes->at(i).remove();
	}
	imgLists->clear();
	delete imgLists;
	imgLists = NULL;

	//Removing vectors
	lists->clear();
	delete lists;
	lists = NULL;

	editboxes->clear();
	delete editboxes;
	editboxes = NULL;
}
void Window::updateScissors()
{
	for(unsigned int i = 0; i < lists->size(); i++)
	{
		lists->at(i).updateScissors();
	}
}

void Window::addList(std::string listID, float offsetX, float offsetY, float width, float height, std::vector< float >* colWidth, std::vector< std::string >* colHeaders)
{
	//Caluclating the position of the list
	float listX = x + offsetX;
	float listY = y + offsetY;

	List tempList;
	tempList.create(listID, listX, listY, width, height, colWidth, colHeaders);
	
	lists->push_back(tempList);
	
}
void Window::addSimpleList(std::string listID, float xPos, float yPos, float width, float height, std::string header)
{
	//Temporary vectors for creating the list
	std::vector< float > colWidth;
	colWidth.push_back(width);
	std::vector< std::string > colHeader;
	colHeader.push_back(header);

	float listX = x + xPos;
	float listY = y + yPos;

	List tempList;
	tempList.create(listID, listX, listY, width, height, &colWidth, &colHeader);

	lists->push_back(tempList);
}
void Window::removeList(std::string listID)
{
	List* list = findListByID(listID);
	
	if(list != NULL) //Making sure the list exists before modifying it
	{
		list->remove();
		
		//Removing the list from the vector
		std::vector< List >::iterator it;
		for(it = lists->begin(); it != lists->end(); it++)
		{
			if( it->getID().compare( listID) == 0)
			{
				lists->erase(it);

				return;
			}
		}
	}
	else
	{
		DebugConsole::addC("Failed to remove list: ");DebugConsole::addC(listID.data());
		DebugConsole::addToLog(". List did not exist");
	}
}
void Window::clearList(std::string listID)
{
	List* list = findListByID(listID);
	
	if(list != NULL) //Making sure the list exists before modifying it
	{
		list->clear();
	}
	else
	{
		DebugConsole::addC("Failed to clear list: ");DebugConsole::addC(listID.data());
		DebugConsole::addToLog(". List did not exist");
	}
}
void Window::addToList(std::string listID, std::vector< std::string >* values)
{
	List* list = findListByID(listID);
	
	if(list != NULL) //Making sure the list exists before modifying it
	{
		list->addItem(values);
	}
	else
	{
		DebugConsole::addC("Failed to add item to the list: ");DebugConsole::addC(listID.data());
		DebugConsole::addToLog(". List did not exist");
	}
}
void Window::addToSimpleList(std::string listID, std::string value)
{
	//Finding the list
	List* list = findListByID(listID);

	if(list != NULL)
	{
		//Creating a vector for the value
		std::vector< std::string > values;
		values.push_back(value);

		list->addItem(&values);
	}
	else
	{
		DebugConsole::addC("Failed to add value to simple list: ");DebugConsole::addC(listID.data());
		DebugConsole::addToLog(" -- List did not exist");
	}

}

void Window::addImgList(std::string vecID, int cloneID, float xPos, float yPos, float width, float height, float imgWidth, float imgHeight, int r, int g, int b, int a)
{
	//Creating a temporary list
	ImgList tempList;
	float listX = x + xPos;
	float listY = y + yPos;
	tempList.create(vecID, cloneID, listX, listY, width, height, imgWidth, imgHeight, r, g, b, a);

	//Add that list to the vector
	imgLists->push_back(tempList);
}
void Window::addImageToImgList(std::string vecID, std::string img, std::string value)
{
	//Getting the list
	ImgList* list = findImgListById(vecID);
	
	if(list != NULL) //Making sure the list exists before modifying it
	{
		list->addImage(img, value);
	}
	else
	{
		DebugConsole::addC("Failed to add image to the Imglist: ");DebugConsole::addC(vecID.data());
		DebugConsole::addToLog(". List did not exist");
	}

}
std::string Window::getImgListSelValue(std::string vecID)
{
	//Getting the list
	ImgList* list = findImgListById(vecID);
	
	if(list != NULL) //Making sure the list exists before modifying it
	{
		return list->getSelectedValue();
	}
	else
	{
		DebugConsole::addC("Failed to add image to the Imglist: ");DebugConsole::addC(vecID.data());
		DebugConsole::addToLog(". List did not exist");
	}
	return "";
}

void Window::addEditbox(std::string vecID, float x, float y, float sizeX, float sizeY)
{
	float xPos = this->x + x;
	float yPos = this->y + y;
	
	UI_editbox tempBox;
	tempBox.create(vecID, xPos, yPos, sizeX, sizeY);

	editboxes->push_back(tempBox);
}
std::string Window::getEditboxValue(std::string vecID)
{
	UI_editbox* editbox = findEditboxById(vecID);

	if(editbox != NULL)
	{
		return editbox->getValue();
	}
	else
	{
		DebugConsole::addC("Failed to get editbox value -- Editbox: ");
		DebugConsole::addC(vecID.data());
		DebugConsole::addC("  did not exist: ");
		DebugConsole::addToLog(this->vecID.data());
	}

	return "";
}
void Window::setEditboxValue(std::string vecID, std::string value)
{
	UI_editbox* editbox = findEditboxById(vecID);

	if(editbox != NULL)
	{
		editbox->setValue(value);
	}
	else
	{
		DebugConsole::addC("Failed to set editbox value -- Editbox: ");
		DebugConsole::addC(vecID.data());
		DebugConsole::addC("  did not exist: ");
		DebugConsole::addToLog(this->vecID.data());
	}
}

void Window::addColorButton(std::string buttonID, float xPos, float yPos, float width, float height, std::string text)
{
	float buttonX = xPos + x;
	float buttonY = yPos + y;
	Button tempButton;

	tempButton.createColorButton(buttonID, buttonX, buttonY, width, height, text);

	buttons->push_back(tempButton);
}
void Window::setButtonColor(std::string buttonID, int r, int g, int b, int hr, int hg, int hb)
{
	Button* button = findButtonById(buttonID);

	if(button != NULL)
	{
		button->setColors(r, g, b, hr, hg, hb);
	}
	else
	{
		DebugConsole::addC("Failed to set button color, button: ");DebugConsole::addC(buttonID.data());
		DebugConsole::addToLog(" did not exist");
	}
}

void Window::setBgColor(int r, int g, int b, int a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

	agk::SetSpriteColor(bgSID, r, g, b, a);
}

std::string Window::getID()
{
	return vecID;
}

List* Window::findListByID(std::string ID)
{
	//Looping thru the list vector
	for(unsigned int i = 0; i < lists->size(); i++)
	{
		if(strcmp(lists->at(i).getID().data(), ID.data()) == 0)
		{
			return &lists->at(i);
		}
	}
	
	return NULL;
}
ImgList* Window::findImgListById(std::string ID)
{
	ImgList* result = NULL;

	for(unsigned int i = 0; i < imgLists->size(); i++)
	{
		if(imgLists->at(i).getVecID().compare(ID) == 0)
		{
			result = &imgLists->at(i);
		}
	}

	return result;
}
UI_editbox* Window::findEditboxById(std::string ID)
{
	UI_editbox* result = NULL;
	for(unsigned int i = 0; i < editboxes->size(); i++)
	{
		if(ID.compare(editboxes->at(i).getVecID()) == 0)
		{
			result = &editboxes->at(i);
		}
	}

	return result;
}
Button* Window::findButtonById(std::string ID)
{
	Button* button = NULL;
	for(unsigned int i = 0; i < buttons->size(); i++)
	{
		if(ID.compare(buttons->at(i).getVecID()) == 0)
		{
			button = &buttons->at(i);
		}
	}

	return button;
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void List::create(std::string vecID, float x, float y, float width, float height, std::vector< float >* colWidth, std::vector< std::string >* colHeaders)
{
	this->vecID = vecID;

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	//Initialising vectors
	listItems = new std::vector< ListItem >;
	this->colWidth = new std::vector< float > (*colWidth); //Cloning the colWidth vector

	//Creating the header
	header.create(colWidth, colHeaders, x, y);

	rowHeight = 13;
	nextItemY = y + rowHeight;
}
void List::remove()
{
	this->header.remove();
	for(unsigned int i = 0; i < listItems->size(); i++)
	{
		listItems->at(i).remove();
	}
	listItems->clear();
	delete listItems;
	listItems = NULL;

	colWidth->clear();
	delete colWidth;
	colWidth = NULL;
}

void List::clear()
{
	for(unsigned int i = 0; i < listItems->size(); i++)
	{
		listItems->at(i).remove();
	}

	nextItemY = y + rowHeight;
	listItems->clear();
}

int List::addItem(std::vector< std::string >* values)
{
	ListItem tempItem; //Creating a tempororary item

	tempItem.create(colWidth, values, x, nextItemY);

	listItems->push_back(tempItem);

	nextItemY = nextItemY + rowHeight;

	return 0;
}
void List::updateInput(float mx, float my)
{
	//Checking if the list is hit
	//Converting the cordinates to world coords
	float startX = agk::ScreenToWorldX(x);
	float startY = agk::ScreenToWorldY(y);
	float endX = agk::ScreenToWorldX(x + width);
	float endY = agk::ScreenToWorldY(y + height);

	//Checking for collision
	if(mx > startX && my > startY && mx < endX && my < endY)
	{
		
	}
}
void List::updateScissors()
{
	for(unsigned int i = 0; i < listItems->size(); i++)
	{
		listItems->at(i).setScissor(agk::ScreenToWorldX(x), agk::ScreenToWorldY(y), agk::ScreenToWorldX(x + width), agk::ScreenToWorldY(y + height));
	}
}

std::string List::getID()
{
	return vecID;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void ListItem::create(std::vector< float >* colWidth, std::vector< std::string >* values, float x, float y)
{
	//initialising vectors
	cells = new std::vector< ListCell >;

	float nextXPos = x; //The next position to create a cell at
	//Going thru the colWidth vector and creating cells
	for(unsigned int i = 0; i < colWidth->size(); i++)
	{
		ListCell tempCell; //Temorary cell

		std::string slotValue;
		if(values->size() > i) //Making sure there is a value for the slot
		{
			slotValue = values->at(i);
		}
		else
		{
			slotValue = "";
		}

		tempCell.create(slotValue, colWidth->at(i), nextXPos, y);

		cells->push_back(tempCell);
		//Calculating the position of the next element
		nextXPos = nextXPos + colWidth->at(i);
	}
}
void ListItem::remove()
{
	for(unsigned int i = 0; i < cells->size(); i++)
	{
		cells->at(i).remove();
	}
	cells->clear();
	delete cells;
	cells = NULL;
}

void ListItem::setScissor(float x1, float y1, float x2, float y2)
{
	for(unsigned int i = 0; i < cells->size(); i++)
	{
		cells->at(i).setScissor(x1, y1, x2, y2); //Setting the scissor for the cell
	}
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void ListCell::create(std::string value, float width, float x, float y)
{
	TID = agk::CreateText( value.data() );
	agk::FixTextToScreen(TID, 1);
	agk::SetTextPosition(TID, x, y);
	agk::SetTextSize(TID, 16);
}
void ListCell::remove()
{
	if(agk::GetTextExists(TID))
	{
		agk::DeleteText(TID);
	}
}

void ListCell::setScissor(float x1, float y1, float x2, float y2)
{
	if(agk::GetTextExists(TID))
	{
		agk::SetTextScissor(TID, x1, y1, x2, y2);
	}
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void ImgList::create(std::string vecID, int cloneID, float x, float y, float width, float height, float imgWidth, float imgHeight, int r, int g, int b, int a)
{
	this->vecID = vecID;
	this->x = x;
	this->y = y;

	this->width = width;
	this->height = height;

	this->imgHeight = imgHeight;
	this->imgWidth = imgWidth;

	//Creating the background
	bgSID = agk::CloneSprite(cloneID);
	agk::FixSpriteToScreen(bgSID, 1);
	agk::SetSpritePosition(bgSID, x, y);
	float bgWidth = width / agk::GetImageWidth(agk::GetSpriteImageID(cloneID));
	float bgHeight = height / agk::GetImageHeight(agk::GetSpriteImageID(cloneID));
	agk::SetSpriteScale(bgSID, bgWidth, bgHeight);
	agk::SetSpriteColor(bgSID, r, g, b, a);

	//Initialising vectors
	listItems = new std::vector< ImgListItem >;
}
void ImgList::updateInput(float mx, float my)
{
	//Checking if the list is hit
	if(mx > agk::ScreenToWorldX(x) && my > agk::ScreenToWorldY(y) && mx < agk::ScreenToWorldX(x + width) && my < agk::ScreenToWorldY(y + height))
	{
		for(unsigned int i = 0; i < listItems->size(); i++)
		{
			bool hit = listItems->at(i).updateInput(mx, my);

			if(hit == true && Input::shoot())
			{
				selected = listItems->at(i).getImage();
			}
		}
	}

	if(selected.compare("") != 0)
	{
		ImgListItem* item = getItemByImage(selected);

		if(item != NULL)
		{
			item->highlight(100, 100, 255);
		}
	}

	//scrolling the menu
	if(Input::scrollAmount() != 0)
	{
		for(unsigned int i = 0; i < listItems->size(); i++)
		{
			listItems->at(i).moveY(Input::scrollAmount() * 5);
		}
	}
}
void ImgList::remove()
{
	if(agk::GetSpriteExists(bgSID))
	{
		agk::DeleteSprite(bgSID);
	}

	for(unsigned int i = 0; i < listItems->size(); i++)
	{
		listItems->at(i).remove();
	}
	//Removing garbage
	listItems->clear();
	delete listItems;
	listItems = NULL;
}

void ImgList::addImage(std::string image, std::string value)
{
	ImgListItem listItem;
	
	//Calculating the position of the new image
	int xAmount = (int) (width - imgWidth * 3) / (int)imgWidth; //The amount of images that fit in one row

	float yPos = (int)(listItems->size() / xAmount) * imgHeight + y;
	int rest = listItems->size() % xAmount;
	float xPos = rest * imgWidth + x;
	
	listItem.create(image, xPos, yPos, imgWidth, imgHeight, value);

	listItems->push_back(listItem);
}

std::string ImgList::getSelectedValue()
{
	ImgListItem* item = getItemByImage(selected);

	std::string result = "";

	if(item != NULL)
	{
		result = item->getValue();
	}

	return result;
}
std::string ImgList::getVecID()
{
	return vecID;
}

ImgListItem* ImgList::getItemByImage(std::string ID)
{
	for(unsigned int i = 0; i < listItems->size(); i++)
	{
		if(listItems->at(i).getImage().compare(ID) == 0)
		{
			return &listItems->at(i);
		}
	}

	return NULL;
}

void ImgListItem::create(std::string image, float x, float y, float width, float height, std::string value)
{
	imgID = agk::LoadImage(image.data());
	SID = agk::CreateSprite(imgID);

	agk::FixSpriteToScreen(SID, 1);

	//Calculating the scale
	float scaleX = width / agk::GetImageWidth(imgID);
	float scaleY = width / agk::GetImageWidth(imgID);
	if(agk::GetImageWidth(imgID) < agk::GetImageHeight(imgID))
	{
		scaleX = height / agk::GetImageHeight(imgID);
		scaleY = height / agk::GetImageHeight(imgID);
	}

	float xPos = x;
	float yPos = y;

	agk::SetSpriteScale(SID, scaleX, scaleY);
	agk::SetSpritePosition(SID, xPos, yPos);

	this->value = value;
	this->image = image;

	this->x = xPos;
	this->y = yPos;

	this->width = width;
	this->height = height;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}
bool ImgListItem::updateInput(float mx, float my)
{
	if(mx > agk::ScreenToWorldX(x) && mx < agk::ScreenToWorldX(x + width) && my > agk::ScreenToWorldY(y) && my < agk::ScreenToWorldY(y + height))
	{
		this->highlight(255, 100, 100);

		return true;
	}
	else
	{
		
	}

	return false;
}
void ImgListItem::remove()
{
	if(agk::GetSpriteExists(SID))
	{
		agk::DeleteSprite(SID);
	}
	if(agk::GetImageExists(imgID))
	{
		agk::DeleteImage(imgID);
	}
}

void ImgListItem::highlight(int r, int g, int b)
{
	agk::DrawLine(x, y, x + width, y, r, g, b);
	agk::DrawLine(x, y, x, y + height, r, g, b);
	agk::DrawLine(x + width, y, x + width, y + height, r, g, b);
	agk::DrawLine(x, y + height, x + width, y + height, r, g, b);
}
void ImgListItem::setColor(int r, int g, int b, int a)
{
	agk::SetSpriteColor(SID, r, g, b, a);
}

std::string ImgListItem::getImage()
{
	return image;
}
std::string ImgListItem::getValue()
{
	return this->value;
}

void ImgListItem::moveY(float amount)
{
	this->y = y + amount;
	
	agk::SetSpritePosition(SID, x, y);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void Button::createColorButton(std::string vecID, float x, float y, float width, float height, std::string text)
{
	type = 1;
	this->vecID = vecID;

	if(agk::GetSpriteExists(1))
	{
		SID = agk::CloneSprite(1);
		agk::FixSpriteToScreen(SID, 1);
		agk::SetSpritePosition(SID, x, y);
		agk::SetSpriteScale(SID, width, height);
		agk::SetSpriteDepth(SID, 1);
		agk::SetSpriteVisible(SID, 1);
		agk::SetSpriteColor(SID, 0, 0, 255, 255);

		//Text
		TID = agk::CreateText(text.data());
		agk::FixTextToScreen(TID, 1);
		agk::SetTextSize(TID, 16);
		agk::SetTextAlignment(TID, 1);
		float tX = x + width / 2;
		float tY = y + height / 2 - 16 / 2;
		agk::SetTextPosition(TID, tX, tY);
		agk::SetTextDepth(TID, 1);
	}
	else
	{
		DebugConsole::addC("Failed to create color button: ");DebugConsole::addC(vecID.data());DebugConsole::addToLog(" Sprite 1 did not exist");
	}
}
void Button::updateInput(float mx, float my)
{
	if( type == 1)
	{
		if(agk::GetSpriteHitTest(SID, mx, my))
		{
			agk::SetSpriteColor(SID, hr, hg, hb, 255);
		}
		else
		{
			agk::SetSpriteColor(SID, r, g, b, 255);
		}
	}
}

void Button::reset()
{
	if(type == 1)
	{
		agk::SetSpriteColor(SID, r, g, b, 255);
	}
}

void Button::setColors(int r, int g, int b, int hr, int hg, int hb)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->hr = hr;
	this->hg = hg;
	this->hb = hb;

	agk::SetSpriteColor(SID, r, g, b, 255);
}

std::string Button::getVecID()
{
	return vecID;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void UI_editbox::create(std::string vecID, float x, float y, float sizeX, float sizeY)
{
	this->vecID = vecID;
	this->x = x;
	this->y = y;
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	ID = agk::CreateEditBox();
	agk::FixEditBoxToScreen(ID, 1);
	agk::SetEditBoxPosition(ID, x, y);
	agk::SetEditBoxSize(ID, sizeX, sizeY);
	agk::SetEditBoxDepth(ID, 1);
	agk::SetEditBoxTextSize(ID, 16);
	agk::SetEditBoxMultiLine(ID, 1);
}
void UI_editbox::remove()
{
	agk::DeleteEditBox(ID);
}
void UI_editbox::setValue(std::string value)
{
	agk::SetEditBoxText(ID, value.data());
}

std::string  UI_editbox::getVecID()
{
	return this->vecID;
}
std::string UI_editbox::getValue()
{
	char* p = agk::GetEditBoxText(ID);;
	std::string result = p;
	delete[] p;
	return result;
}