#include "ExitCommand.h"

#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

ExitCommand::ExitCommand(std::string argsString) : Command(argsString)
{
	invokeName = "exit";
	description = "Exit the system safely.";
	usage = "";
	successTip = "exit the system!";
}

void ExitCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	SystemCenterFacade::exitSystem();
}
