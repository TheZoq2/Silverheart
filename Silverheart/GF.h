#pragma once

#include "agk.h"
#include <string>

//Global variables
#ifndef _MY_GLOBALS_
#define _MY_GLOBALS_

extern float globaltime;
extern float oldTime;
extern float frameTime;
extern float speedMod;
extern int GF_BaseDepth;
extern int GF_charGroup;
extern float physScale;

#endif


class GF
{
public:
	GF(void);
	~GF(void);

	static char sdPath[128];
	static int quality;

	static void setupGlobals();

	static std::string getPath(char originalPath[128]); //Returns the path to a file with medium quality
	static std::string getPath(std::string filename);

	static float tileSize; //The size of each tile in the world

	static void updateTime();

	static void changeState(bool* var); //This function will take a boolean and change the state of it (false > true, true > false)

	/*static float time;
	static float oldTime;
	static float frameTime;
	static float speedMod;*/
};

