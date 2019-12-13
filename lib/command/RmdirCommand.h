#pragma once
#include "Command.h"
class RmdirCommand :
	public Command
{
public:
	RmdirCommand(std::string argsString);
	void execute();
private:
	std::string getDirName();
};

