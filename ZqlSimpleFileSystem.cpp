#include <iostream>
#include "lib/cli/Tip.h"
#include "lib/cli/CLIHandler.h"

int main() {
	Tip::outputWelcomeTips();

	CLIHandler cliHandler;
	cliHandler.receiveUserInputUtilQuit();

	system("pause");
}