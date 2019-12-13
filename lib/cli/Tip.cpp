#include "Tip.h"
#include <iostream>

std::string Tip::WELCOME_TIPS = "Welcome to use Zhangql Simple File System!\n\n"
	"Input \"help\" after your command to get the tip of the command, such as \"new help\", \"ls help\".\n"
	"Input \"quit\" to quit the program safely. \n"
	"If you close the command window directly, "
	"the running file system will not be saved!";
std::string Tip::OUTPUT_DIVIDE_LINES = "------------------------------------------------------";
std::string Tip::COMMAND_INPUT_SIGN = "> ";
std::string Tip::NO_SUCH_COMMAND_ERROR = "Sorry, there's no such command in the system.";

void Tip::outputWelcomeTips() {
	std::cout << Tip::OUTPUT_DIVIDE_LINES << std::endl;
	std::cout << Tip::WELCOME_TIPS << std::endl;
	std::cout << Tip::OUTPUT_DIVIDE_LINES << std::endl;
}