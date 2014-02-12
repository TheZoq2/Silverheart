#pragma once

#include "agk.h"

#include <algorithm>
#include <vector>

class DataReader
{
public:
	DataReader(void);
	~DataReader(void);

	static std::string getType(std::string line);
	static std::string getValue(std::string line);
	static std::string getValue(std::string line, int pos);
	static int getValueAmount(std::string line);

	static std::vector< std::string > getArray(std::string data);
};

