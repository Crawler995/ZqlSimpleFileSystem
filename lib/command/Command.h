#pragma once

#include <string>
#include <vector>


class Command
{
public:
	std::string invokeName;
	std::string description;
	std::string usage;
	std::string successTip;

	Command(std::string argsString);

	virtual void execute() = 0;

protected:
	std::vector<std::string> args;
	bool isRequestHelp();
	void outputHelp();

private:
	void parseArgsString(std::string argsString);
};

