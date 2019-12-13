#include "CloseCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

CloseCommand::CloseCommand(std::string argsString) : Command(argsString)
{
	invokeName = "close";
	description = "Close a opening file.";
	usage = "";
	successTip = "";
}

void CloseCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	SystemCenterFacade::close();
}