#pragma once


#include "Command.h"

class ExitCommand :
	public Command
{
public:
	ExitCommand(std::string argsString);
	void execute();
};

