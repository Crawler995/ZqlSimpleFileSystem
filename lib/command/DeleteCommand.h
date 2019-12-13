#pragma once
#include "Command.h"
class DeleteCommand :
	public Command
{
public:
	DeleteCommand(std::string argsString);
	void execute();
private:
	std::string getFileName();
};

