#include "CreateCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

CreateCommand::CreateCommand(std::string argsString) : Command(argsString)
{
	invokeName = "create";
	description = "Create a new empty file.";
	usage = "create <file name (len(fileName) < 55)>";
	successTip = "";
}

void CreateCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	if (getFileName() == "") {
		std::cout << "Error: argument error! Usage: " << usage << std::endl;
		return;
	}

	SystemCenterFacade::create(getFileName());
}

std::string CreateCommand::getFileName() {
	if (args.size() != 2) {
		return "";
	}
	if (args[1].size() > 55) {
		return "";
	}

	return args[1];
}