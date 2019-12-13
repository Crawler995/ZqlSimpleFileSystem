#include "NewCommand.h"
#include "../data_structure/SystemCenterFacade.h"

#include <iostream>

NewCommand::NewCommand(std::string argsString) : Command(argsString)
{
	invokeName = "new";
	description = "Create a new system.";
	usage = "new <system memory size (>200)(KB)>";
	successTip = "create new system successfully!";

	systemMemorySize = getSystemMemorySize();
}

void NewCommand::execute()
{
	if (isRequestHelp()) {
		outputHelp();
		return;
	}

	if (systemMemorySize < 200) {
		std::cout << "Error: illegal size! Usage: " << usage << std::endl;
		return;
	}

	SystemCenterFacade::newSystem(systemMemorySize);
}

int NewCommand::getSystemMemorySize() {
	if (args.size() < 2) {
		return -1;
	}

	return atoi(args[1].c_str());
}