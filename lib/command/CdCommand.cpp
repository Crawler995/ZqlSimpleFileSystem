#include "CdCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

CdCommand::CdCommand(std::string argsString) : Command(argsString)
{
	invokeName = "cd";
	description = "change current directory.";
	usage = "cd <folder name>";
	successTip = "";
}

void CdCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	if (getDestDirName() == "") {
		std::cout << "Error: need one argument! Usage: " << usage << std::endl;
		return;
	}

	SystemCenterFacade::cd(getDestDirName());
}

std::string CdCommand::getDestDirName()
{
	if (args.size() != 2) {
		return "";
	}

	std::string res = args[1];

	if (res[res.size() - 1] == '/') {
		res.erase(res.end() - 1);
	}

	return res;
}