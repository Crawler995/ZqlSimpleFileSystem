#include "LsCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

LsCommand::LsCommand(std::string argsString) : Command(argsString)
{
	invokeName = "ls";
	description = "list all files or directories in current directory.";
	usage = "";
	successTip = "";
}

void LsCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	SystemCenterFacade::ls();
}
