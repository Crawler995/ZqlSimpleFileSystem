#include "RmdirCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

RmdirCommand::RmdirCommand(std::string argsString) : Command(argsString)
{
	invokeName = "rmdir";
	description = "Remove a directory.";
	usage = "rmdir <folder name>";
	successTip = "";
}

void RmdirCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	std::string dirName = getDirName();
	if (dirName == "") {
		std::cout << "Error: argument error! Usage: " << usage << std::endl;
		return;
	}

	SystemCenterFacade::rmdir(dirName);
}

std::string RmdirCommand::getDirName()
{
	if (args.size() != 2) {
		return "";
	}

	return args[1];
}