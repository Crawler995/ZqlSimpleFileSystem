#pragma once
#include "Command.h"
class OpenCommand :
	public Command
{
public:
	OpenCommand(std::string argsString);
	void execute();
private:
	std::string getFileName();
};

