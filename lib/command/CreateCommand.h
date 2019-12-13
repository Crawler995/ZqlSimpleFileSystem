#pragma once
#include "Command.h"
class CreateCommand :
	public Command
{
public:
	CreateCommand(std::string argsString);
	void execute();
private:
	std::string getFileName();
};

