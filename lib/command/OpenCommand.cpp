#include "OpenCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

OpenCommand::OpenCommand(std::string argsString) : Command(argsString)
{
	invokeName = "open";
	description = "open a existed file to read or write.";
	usage = "open <file name>";
	successTip = "";
}

void OpenCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	if (getFileName() == "") {
		std::cout << "Error: argument error! Usage: " << usage << std::endl;
		return;
	}

	SystemCenterFacade::open(getFileName());
}

std::string OpenCommand::getFileName() {
	if (args.size() != 2) {
		return "";
	}

	return args[1];
}