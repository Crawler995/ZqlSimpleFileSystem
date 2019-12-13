#pragma once

#include "Command.h"


class QuitCommand :
	public Command
{
public:
	QuitCommand(std::string argsString);
	void execute();
};

