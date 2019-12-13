#pragma once
#include "Command.h"
class SfsCommand :
	public Command
{
public:
	SfsCommand(std::string argsString);
	void execute();
private:
	std::string getFileName();
};

