#include "DataReader.h"

#include "DebugConsole.h"

DataReader::DataReader(void)
{
}


DataReader::~DataReader(void)
{
}

std::string DataReader::getType(std::string line)
{
	std::string type;

	//Checking if this is a comment
	std::string comment = "//";
	std::string commentChk;
	commentChk = line.substr(0, 2);
	if(comment.compare(commentChk) == 0) //This wasn't a comment, check for commands
	{
		type = "comment";
	}
	else //This was a comment
	{
		//Finding the : on the line
		std::size_t colon = line.find(':');
		//Checking if the colon is actually there
		if(colon != std::string::npos)
		{
			type = line.substr(0, colon);
		}
		else
		{
			type = "error_typeNotFound";
		}
	}

	return type;
}

std::string DataReader::getValue(std::string line)
{
	std::string value;
	//Finding the colon which is the start of the character
	int colon = line.find(':');

	if(colon != -1) //Making sure there actually is a colon on  the line
	{
		value = line.substr(colon + 1, std::string::npos); //Returning the whole line value
	}
	else
	{
		//Since a user friendly error is returned when finding the colon in the type function,
		//This function will add a simpler error to the log
		DebugConsole::addC("Colon not found when getting value from line: ");
		DebugConsole::addToLog(line);
	}

	return value;
}
std::string DataReader::getValue(std::string line, int pos)
{
	std::string value;
	//Counting the amount of colons
	int colon = line.find(':');

	line = line.substr(colon + 1, std::string::npos);
	for(int i = 0; i <= pos; i++)
	{
		int comma = line.find(',');
		value = line.substr(0, comma); //Getting the value

		//Removing the already found parameters
		line = line.substr(comma + 1, std::string::npos);
	}

	return value;
}

int DataReader::getValueAmount(std::string line)
{
	//return line.Count(',');
	return std::count(line.begin(), line.end(), ',');
}

std::vector<std::string> DataReader::getArray(std::string data)
{
	std::vector<std::string> result;
	
	//Finding the first [
	std::size_t lastEnd = 0;

	bool finished = false;
	while(finished == false)
	{
		std::size_t startPos = data.find("[", lastEnd + 1);
		std::size_t endPos = data.find("]", lastEnd + 1);

		if(startPos == std::string::npos)
		{
			finished = true;
			break;
		}

		if(endPos == std::string::npos)
		{
			finished = true;

			DebugConsole::addToLog("Failed to read array part, missing ]");

			break;
		}

		result.push_back(data.substr(startPos + 1, endPos - startPos - 1));

		lastEnd = endPos;
	}

	return result;
}