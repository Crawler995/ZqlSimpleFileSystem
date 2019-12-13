#include "SfsCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

SfsCommand::SfsCommand(std::string argsString) : Command(argsString)
{
	invokeName = "sfs";
	description = "Recover a system from cache file.";
	usage = "sfs <file name>";
	successTip = "";
}

void SfsCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	if (getFileName() == "") {
		std::cout << "Error: argument error! Usage: " << usage << std::endl;
		return;
	}

	SystemCenterFacade::sfs(getFileName());
}

std::string SfsCommand::getFileName() {
	if (args.size() != 2) {
		return "";
	}

	return args[1];
}