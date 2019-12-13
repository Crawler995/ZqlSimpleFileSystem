#pragma once
#include "Command.h"
class MkdirCommand :
	public Command
{
public:
	MkdirCommand(std::string argsString);
	void execute();

private:
	std::string getDirName();
};

