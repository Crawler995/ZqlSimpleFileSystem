#include "DeleteCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

DeleteCommand::DeleteCommand(std::string argsString) : Command(argsString)
{
	invokeName = "delete";
	description = "Delete a existed file.";
	usage = "delete <file name>";
	successTip = "";
}

void DeleteCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	if (getFileName() == "") {
		std::cout << "Error: argument error! Usage: " << usage << std::endl;
		return;
	}

	SystemCenterFacade::ddelete(getFileName());
}

std::string DeleteCommand::getFileName() {
	if (args.size() != 2) {
		return "";
	}

	return args[1];
}