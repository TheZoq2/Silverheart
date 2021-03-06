// Includes
#include "template.h"

// Namespace
using namespace AGK;

app App;

void app::Begin(void)
{
	LuaHandler::setupLua();

	//Setting a random seed for the c++ random function
	srand(unsigned int( time(NULL) ));

	GF::setupGlobals();

	agk::SetWindowTitle("MedievalPlat");
	agk::SetVirtualResolution (agk::GetDeviceWidth(), agk::GetDeviceHeight());
	agk::SetSyncRate(0,1);
	agk::SetClearColor(0, 0, 0);

	agk::SetPrintSize(15);
	agk::SetPrintColor(255, 0, 0, 255);

	worldSim.setup();
	world.begin();

	player.begin(&world);

	Input::setup();

	npcGroup.setup();

	projGroup.setup();

	mainUI.setup();

	//Setting up the item generator
	ItemGen::setup();

	fps = true;

	//Creating the 1x1 sprite
	agk::LoadImage(1, GF::getPath("1x1.png").data());
	agk::CreateSprite(1, 1);
	agk::SetSpriteVisible(1, 0);

	DebugConsole::setup();

	//Registering LUA functions
	l_defaultPlr = &player;
	l_defaultWorld = &world;
	l_defaultNPCgroup = &npcGroup;
	l_defaultUI = &mainUI;

	defaultNPCGroup = &npcGroup;

	//Physics setup
	physScale = 0.01f;
	agk::SetPhysicsScale(physScale);

	int testText = agk::CreateText("Hello world");
	agk::FixTextToScreen(testText, 0);
	agk::SetTextSize(testText, 16);
	agk::SetTextVisible(testText, 1);
}

void app::Loop (void)
{
	Input::updateInput();
	GF::updateTime();
	mainUI.update();

	//Main menu
	if(programState == 0)
	{
		programState = 2; //Starting to load a world
	}
	else if(programState == 1)
	{
		//Main game loop
		player.update();
		player.updateWeapon(&projGroup);

		world.update(player.getX(), player.getY());

		//IngameMenu::update();

		npcGroup.update(&world);
		npcGroup.updateChars(&npcGroup, &player);

		projGroup.update( player.getX(), player.getY());
		projGroup.updateWorld(&world);

		mainUI.updateScissors();
		//worldSim.viewRegions(&mainUI);
		//worldSim.viewFactions(&mainUI);
	}
	else if(programState == 2)
	{	
		world.loadBaseMedia();
		world.setTime(800);

		player.load(GF::getPath("Bound.png"));

		LuaHandler::runScript("scripts/firstspawn.lua");

		programState = 1;
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
		agk::PrintC("Active keystate: "); agk::Print(Input::activate());
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
		DebugInput::update(&world, &player, &npcGroup);
	}
	else
	{
		DebugConsole::setVisible(0);
	}

	//Syncing

	agk::Update2D(); //Updating the game
	/////////////////////////////////////////////////////////////////////////
	//							Light calculation
	/////////////////////////////////////////////////////////////////////////'

	/*
	//Turning the light mode on
	world.setLightModeOn();

	agk::Render(); //Rendering the image
	//Changing the display aspect
	float lightScale = 10.0f;
	agk::SetViewZoom(15 / lightScale);

	//Calculating the cordinates of the new image
	float h = agk::GetVirtualHeight(); float w = agk::GetVirtualWidth();
	float imgHeight = h / lightScale;
	float imgWidth = w / lightScale;

	//Getting the image
	int lightImg = agk::GetImage(w / 2.0f - imgWidth / 2.0f, h / 2.0f - imgHeight / 2.0f, imgWidth, imgHeight);

	int sprite = agk::CreateSprite(lightImg);
	agk::FixSpriteToScreen(sprite, 1);
	//agk::SetSpriteScale(sprite, lightScale, lightScale);

	//Resetting the world
	world.setLightModeOff();*/
	//agk::SetViewZoom(15);

	//calculateLight();
	/////////////////////////////////////////////////////////////////////////

	//agk::Render();

	//agk::Render2DBack();
	agk::Render3D();
	agk::Render2DBack();
	agk::Render2DFront();
	agk::Swap();

	/*agk::DeleteImage(lightImg);
	agk::DeleteSprite(sprite);*/
}

void app::End (void)
{
	
}

void app::calculateLight()
{
	/*
	//Turning the light mode on
	world.setLightModeOn();

	agk::Render2DFront(); //Rendering the image
	//Changing the display aspect
	float lightScale = 10.0f;
	agk::SetViewZoom(15 / lightScale);

	//Calculating the cordinates of the new image
	float h = agk::GetVirtualHeight(); float w = agk::GetVirtualWidth();
	float imgHeight = h / lightScale;
	float imgWidth = w / lightScale;

	//Getting the image
	int lightImg = agk::GetImage(w / 2.0f - imgWidth / 2.0f, h / 2.0f - imgHeight / 2.0f, imgWidth, imgHeight);

	agk::ClearScreen();

	int sprite = agk::CreateSprite(lightImg);
	agk::FixSpriteToScreen(sprite, 1);
	//agk::SetSpriteScale(sprite, lightScale, lightScale);

	//Resetting the world
	world.setLightModeOff();

	agk::SetViewZoom(15);
	*/
}