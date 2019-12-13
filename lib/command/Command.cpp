#include "Command.h"

#include <iostream>

Command::Command(std::string argsString)
{
	parseArgsString(argsString);
}

void Command::parseArgsString(std::string argsString)
{
	std::string tmp;
	for (int i = 0; i < argsString.size(); i++) {
		if (argsString[i] == ' ') {
			args.push_back(tmp);
			tmp.erase();
			continue;
		}

		tmp.push_back(argsString[i]);
	}

	args.push_back(tmp);
}

bool Command::isRequestHelp() {
	return args.size() == 2 && args[1] == "help";
}

void Command::outputHelp() {
	std::cout << "Help: " << description;
	if (usage != "") {
		std::cout << " Usage: " << usage;
	}
	std::cout << std::endl;
}
