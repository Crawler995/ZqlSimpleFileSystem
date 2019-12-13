#pragma once
#include "Command.h"
class NewCommand :
	public Command
{
public:
	NewCommand(std::string argsString);
	void execute();

private:
	int systemMemorySize;

	int getSystemMemorySize();
};

