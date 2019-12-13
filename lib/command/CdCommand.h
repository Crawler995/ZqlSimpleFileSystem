#pragma once
#include "Command.h"
class CdCommand :
	public Command
{
public:
	CdCommand(std::string argsString);
	void execute();

private:
	std::string getDestDirName();
};

