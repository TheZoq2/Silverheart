#pragma once

#include"agk.h"

#include "GF.h"
#include "World.h"
#include "Input.h"
#include "Item.h"
#include "Weapon.h"
#include "Character.h"

#include "LUA/lua.hpp"

//#include "Script.h"

class Player
{
public:
	Player(void);
	~Player(void);
	
	void begin(World* world);

	void load(std::string name);
	void setupLuaFunctions();

	void spawn(std::string point);

	void setPosition(float x, float y);

	void update();
	void updateWeapon(ProjectileGroup* projGroup);
	void activation();

	bool canTravel();
	void setJustLoaded(bool justLoaded);
	void setVisible(int visible);

	void addItem(Item item);
	void setCurrentWeaponByName(std::string name);

	float getX();
	float getY();

	int getInvAmount();
	Item getItemFromSlot(int slot);
private:
	int img;
	int SID;

	Character chr;

	int activateText;
	int activateSprite;

	float x;
	float y;

	float weapOffsetX;
	float weapOffsetY;

	float cameraX;
	float cameraY;

	int health;

	float lastJump;

	float speed;
	float jumpHeight;

	World* world;

	float spriteScale;

	bool checkOnGround();

	bool justLoaded;
	float lastTravel;

	std::vector< Item >* inventory;

	Weapon cWeapon;

	struct Animation
	{
		int SID;
		int imgID;
		bool animated;

		int FPS;
		int start;
		int end;
	};

	Animation anim[2];

	//animations
	int skeleton;
	int atlasImg;

	//Arm arm;
};

