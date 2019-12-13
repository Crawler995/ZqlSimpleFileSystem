#include "MkdirCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

MkdirCommand::MkdirCommand(std::string argsString) : Command(argsString)
{
	invokeName = "mkdir";
	description = "Make a directory.";
	usage = "mkdir <folder name>";
	successTip = "make new dir successfully!";
}

void MkdirCommand::execute()
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

	SystemCenterFacade::mkdir(dirName);
}

std::string MkdirCommand::getDirName()
{
	if (args.size() != 2) {
		return "";
	}

	return args[1];
}
