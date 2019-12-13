#include "WriteCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

WriteCommand::WriteCommand(std::string argsString) : Command(argsString)
{
	invokeName = "write";
	description = "Write a opening file.";
	usage = "";
	successTip = "";
}

void WriteCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	SystemCenterFacade::write();
}