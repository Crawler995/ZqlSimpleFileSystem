#include "ReadCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

ReadCommand::ReadCommand(std::string argsString) : Command(argsString)
{
	invokeName = "read";
	description = "Read the opening file.";
	usage = "";
	successTip = "";
}

void ReadCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	SystemCenterFacade::read();
}