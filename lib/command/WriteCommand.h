#pragma once
#include "Command.h"
class WriteCommand :
	public Command
{
public:
	WriteCommand(std::string argsString);
	void execute();
};

