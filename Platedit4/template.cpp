// Includes
#include "template.h"

// Namespace
using namespace AGK;

app App;

void app::Begin(void)
{
	agk::SetVirtualResolution (1600, 900);
	agk::SetClearColor( 151,170,204 ); // light blue
	agk::SetSyncRate(60,0);
	agk::SetScissor(0,0,0,0);
	agk::SetPrintSize(16);
	agk::SetPrintColor(255, 0, 0);

	//Loading the 1x1 sprite
	agk::LoadImage(1,"media/1x1.png");
	agk::CreateSprite(1, 1);
	agk::SetSpriteVisible(1, 0);

	//Class setup
	GF::setupGlobals();

	DebugConsole::setup();
	uiGroup.setup();

	editor.setup();
}

void app::Loop (void)
{
	GF::updateTime();
	Input::updateInput();

	uiGroup.update();
	uiGroup.updateScissors();
	
	agk::StepPhysics(0);

	if(programState == 0) //Loading stuff
	{
		//Setting up the menus
		uiGroup.addWindow("toolbar", "1x1.png", 0, 0, 250.0f, (float)agk::GetVirtualHeight());
		uiGroup.setWindowColor("toolbar", 150, 150, 150, 255);
		std::vector< float > colWidth;
		colWidth.push_back(120.0f);
		colWidth.push_back(40.0f);
		std::vector< std::string > headers;
		headers.push_back("Parts");headers.push_back("key");
		uiGroup.addListToWindow("toolbar", "toolList", 5.0f, 5.0f, 240.0f, agk::GetVirtualHeight() - 5.0f, &colWidth, &headers);

		std::vector< std::string > tool;
		tool.push_back("");
		tool.push_back("");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Partpicker");
		tool.push_back("p");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("");
		tool.push_back("");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Select");
		tool.push_back("space");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Create");
		tool.push_back("c");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Move");
		tool.push_back("m");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Scale");
		tool.push_back("s");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Rotate");
		tool.push_back("r");
		uiGroup.addToList("toolbar", "toolList", &tool);
		
		tool.clear();
		tool.push_back("");
		tool.push_back("");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Physics +");
		tool.push_back(".");
		uiGroup.addToList("toolbar", "toolList", &tool);
		tool.clear();
		tool.push_back("Physics -");
		tool.push_back(",");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Depth +");
		tool.push_back("up");
		uiGroup.addToList("toolbar", "toolList", &tool);
		tool.clear();
		tool.push_back("Depth -");
		tool.push_back("down");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("");
		tool.push_back("");
		uiGroup.addToList("toolbar", "toolList", &tool);
	
		tool.clear();
		tool.push_back("Waypoints");
		tool.push_back("");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Add node");
		tool.push_back("n");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Move node");
		tool.push_back("b");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Jump link");
		tool.push_back("j");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("");
		tool.push_back("");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Save");
		tool.push_back("ctrl+s");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Load");
		tool.push_back("ctrl+l");
		uiGroup.addToList("toolbar", "toolList", &tool);

		programState = 1;
	}
	if(programState == 1)
	{
		if(uiGroup.getUIActive() == false)
		{
			toolInput();
		}
		updateMenus();

		editor.update(selTool, uiGroup.getUIActive());
		editor.updateSelectedWindow(&uiGroup);
	}
	
	///////////////////////////////////////////////////////////////
	//						Debug output
	///////////////////////////////////////////////////////////////
	if(agk::GetRawKeyPressed(112))
	{
		GF::changeState(&fps);
	}

	if(agk::GetRawKeyPressed(113))
	{
		GF::changeState(&debug);
	}

	if(agk::GetRawKeyPressed(114)){ GF::changeState(&physDebug); }

	//Printing fps info
	if(fps)
	{
		agk::PrintC("FPS:  ");agk::Print(agk::ScreenFPS());
	}

	//Printing debug info
	if(debug == true)
	{
		agk::Print("");
		agk::PrintC("Physics time: ");agk::Print(agk::GetPhysicsTime());
		agk::PrintC("Drawing setup time: ");agk::Print(agk::GetDrawingSetupTime());
		agk::PrintC("Drawing time: ");agk::Print(agk::GetDrawingTime());
		agk::PrintC("Managed sprites: ");agk::Print(agk::GetManagedSpriteCount());
		agk::Print("");
		agk::PrintC("Speedmod: ");agk::Print(speedMod);
		agk::PrintC("Last keystate: ");agk::Print(agk::GetRawLastKey()); 
		agk::PrintC("Pointer x: ");agk::PrintC(i_mx);agk::PrintC("   PointerY: ");agk::Print(i_my);
		agk::PrintC("View zoom: ");agk::Print(agk::GetViewZoom());
		agk::PrintC("Global time: ");agk::Print(globaltime);
	}

	if(physDebug == true)
	{
		agk::SetPhysicsDebugOn();
	}else
	{
		agk::SetPhysicsDebugOff();
	}

	//Updating the debug console
	if(Input::debugKey())
	{
		GF::changeState(&console);
	}

	if(console == true)
	{
		DebugConsole::setVisible(1);
	}
	else
	{
		DebugConsole::setVisible(0);
	}

	agk::Sync();
}

void app::End (void)
{

}

void app::toolInput()
{
	if(agk::GetRawKeyPressed(80))
	{
		//part picker
		if(uiGroup.getWindowExists("partpicker") == false && uiGroup.getWindowExists("saving") == false && uiGroup.getWindowExists("loading") == false)
		{
			uiGroup.addWindow("partpicker", "1x1.png", 250.0f, 0, (float)agk::GetVirtualWidth() - 250.0f, (float)agk::GetVirtualHeight());
			uiGroup.setWindowColor("partpicker", 150, 150, 150, 255);
			//Adding the image list to the window
			uiGroup.addImgListToWindow("partpicker", "imgList", 1, 360, 5, (float)agk::GetVirtualWidth() - 350 - 10, (float)agk::GetVirtualHeight() - 10, 100, 100, 255, 255, 255, 150);
		
			//Adding the folder list
			uiGroup.addSimpleListToWindow("partpicker", "folderList", 5, 5, 100, (float) agk::GetVirtualHeight() - 10, "Folder");

			//Reading all images in the media folder
			std::string mediaFolder = "media";

			std::string oldDir = agk::GetCurrentDir();
			agk::SetCurrentDir("media");
			
			std::string file = agk::GetFirstFile();
			while(file.compare("") != 0)
			{
				std::string fullFilepath = mediaFolder;
				fullFilepath.append("/");
				fullFilepath.append(file);
				std::string filepath = file;

				uiGroup.addImageToImgList("partpicker", "imgList", filepath, fullFilepath);

				file = agk::GetNextFile();
			}

			//Reading folers
			std::string folder = agk::GetFirstFolder();
			while(folder.compare("") != 0)
			{
				uiGroup.addToSimpleList("partpicker", "folderList", folder.data());
				
				folder = agk::GetNextFolder();
			}

			agk::SetCurrentDir(oldDir.data());
		}
	}

	//If the ctrl key is not pressed
	if(agk::GetRawKeyState(17) == 0)
	{
		if(agk::GetRawKeyPressed(67))
		{
			selTool = tool_place;
		}
		if(agk::GetRawKeyState(32))
		{
			selTool = tool_select;
		}
		if(agk::GetRawKeyState(77))
		{
			selTool = tool_move;
		}
		if(agk::GetRawKeyState(83))
		{
			selTool = tool_scale;
		}
		if(agk::GetRawKeyState(82))
		{
			selTool = tool_rotate;
		}
		if(agk::GetRawKeyState(78)) //Node creation
		{
			selTool = tool_createNode;
		}
		if(agk::GetRawKeyState(66)) //Node move
		{
			selTool = TOOL_MOVENODE;
		}
		if(agk::GetRawKeyState(74)) //Jump links
		{
			selTool = TOOL_JUMPLINK;
		}
		if(agk::GetRawKeyState(76)) //Regular link
		{
			selTool = TOOL_LINK;
		}


		if(agk::GetRawKeyPressed(188))
		{
			editor.incPhysState(-1);
		}
		if(agk::GetRawKeyPressed(190))
		{
			editor.incPhysState(1);
		}

		if(agk::GetRawKeyPressed(38))
		{
			editor.incDepth(1);
		}
		if(agk::GetRawKeyPressed(40))
		{
			editor.incDepth(-1);
		}

		if(agk::GetRawKeyPressed(46))
		{
			editor.removeSelected();
		}

		if(agk::GetRawKeyPressed(9))
		{
			editor.toggleSnapping();
		}
	}
	else if(agk::GetRawKeyState(17))
	{
		if(agk::GetRawKeyState(67))
		{
			editor.copy();
		}
		if(agk::GetRawKeyPressed(86))
		{
			editor.paste(i_mx, i_my);
		}

		if(agk::GetRawKeyState(48))
		{
			agk::Print("resetting zoom");
			editor.resetZoom();
		}

		//Saving
		if(agk::GetRawKeyState(83))
		{
			//Creating the save window
			if(uiGroup.getWindowExists("saving") == 0)
			{
				uiGroup.addWindow("saving", "1x1.png", (float)agk::GetVirtualWidth() / 2.0f - 250, (float)agk::GetVirtualHeight() / 2.0f - 100, 500, 200);
				uiGroup.setWindowColor("saving", 150, 150, 150, 255);

				uiGroup.addEditboxToWindow("saving", "filename", 30, 80, 440, 25);
			}
		}
		if(agk::GetRawKeyState(76))
		{
			if(uiGroup.getWindowExists("loading") == 0)
			{
				uiGroup.addWindow("loading", "1x1.png", agk::GetVirtualWidth() / 2 - 250, agk::GetVirtualHeight() / 2 - 100, 500, 200);
				uiGroup.setWindowColor("loading", 150, 150, 150, 100);

				uiGroup.addEditboxToWindow("loading", "filename", 30, 80, 440, 25);
			}
		}
	}
}

void app::updateMenus()
{
	if(uiGroup.getWindowExists("partpicker"))
	{
		std::string selectedValue = uiGroup.getImgListSelValue("partpicker", "imgList");
		if(selectedValue.compare("") != 0)
		{
			//Save the value
			editor.setSelImage(selectedValue);

			uiGroup.removeWindow("partpicker");
		}
	}
	if(uiGroup.getWindowExists("saving"))
	{
		//If the user preses enter
		if(agk::GetRawKeyState(13))
		{
			//Getting the value of the editbox
			std::string value = uiGroup.getEditboxValue("saving", "filename");

			editor.saveMap(value);
			uiGroup.removeWindow("saving");
		}
	}

	if(uiGroup.getWindowExists("loading"))
	{
		//If the user preses enter
		if(agk::GetRawKeyState(13))
		{
			//Getting the value of the editbox
			std::string value = uiGroup.getEditboxValue("loading", "filename");
			
			editor.loadMap(value);
			uiGroup.removeWindow("loading");
		}
	}
}