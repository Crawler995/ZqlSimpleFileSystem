#pragma once
#include "Command.h"
class CloseCommand :
	public Command
{
public:
	CloseCommand(std::string argsString);
	void execute();
};

