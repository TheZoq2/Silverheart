#pragma once

#include "agk.h"
#include <string>

#ifndef DC_GLOBALS
#define DC_GLOBALS
	extern int  dc_textID;
	extern int dc_editID;
	extern int dc_bgSID;

	extern int dc_visible;

	extern std::string consoleText;
#endif

class DebugConsole
{
public:
	DebugConsole(void);
	~DebugConsole(void);

	static void setup();

	static void setVisible(int visible);

	static void addToLog(std::string msg);
	static void addC(std::string msg);

	static bool isActive();
};

