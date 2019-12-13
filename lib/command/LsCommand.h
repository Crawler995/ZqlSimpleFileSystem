#pragma once
#include "Command.h"
class LsCommand :
	public Command
{
public:
	LsCommand(std::string argsString);
	void execute();
};

