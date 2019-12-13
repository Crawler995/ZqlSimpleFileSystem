#pragma once

#include <string>
#include <iostream>

#include "Tip.h"


class CLIHandler
{
public:
	void receiveUserInputUtilQuit();

private:
	std::string getInvokeCommandName(std::string userInput);
};

