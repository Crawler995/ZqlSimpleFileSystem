#include "QuitCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

QuitCommand::QuitCommand(std::string argsString) : Command(argsString)
{
	invokeName = "quit";
	description = "Exit the system safely and stop the program.";
	usage = "";
	successTip = "quit the program!";
}

void QuitCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	SystemCenterFacade::quitSystem();
}
