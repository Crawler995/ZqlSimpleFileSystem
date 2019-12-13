#pragma once
#include "Command.h"
class ReadCommand :
	public Command
{
public:
	ReadCommand(std::string argsString);
	void execute();
};

