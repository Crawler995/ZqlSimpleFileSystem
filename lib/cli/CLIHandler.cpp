#include "CLIHandler.h"
#include "../command/QuitCommand.h"
#include "../command/NewCommand.h"
#include "../command/ExitCommand.h"
#include "../command/MkdirCommand.h"
#include "../command/RmdirCommand.h"
#include "../command/LsCommand.h"
#include "../command/CdCommand.h"
#include "../command/CreateCommand.h"
#include "../command/OpenCommand.h"
#include "../command/CloseCommand.h"
#include "../command/WriteCommand.h"
#include "../command/ReadCommand.h"
#include "../command/DeleteCommand.h"
#include "../command/SfsCommand.h"
#include "../data_structure/SystemCenterFacade.h"

void CLIHandler::receiveUserInputUtilQuit() {
	while (true) {
		std::cout << std::endl << SystemCenterFacade::getCurPath() << Tip::COMMAND_INPUT_SIGN;
		std::string userInput;
		std::getline(std::cin, userInput);

		std::string invokeCommandName = getInvokeCommandName(userInput);

		if (invokeCommandName == "new") {
			NewCommand newCommand(userInput);
			newCommand.execute();
		}

		else if (invokeCommandName == "quit") {
			QuitCommand quitCommand(userInput);
			quitCommand.execute();

			return;
		}

		else if (invokeCommandName == "exit") {
			ExitCommand exitCommand(userInput);
			exitCommand.execute();
		}

		else if (invokeCommandName == "mkdir") {
			MkdirCommand mkdirCommand(userInput);
			mkdirCommand.execute();
		}

		else if (invokeCommandName == "ls") {
			LsCommand lsCommand(userInput);
			lsCommand.execute();
		}

		else if (invokeCommandName == "cd") {
			CdCommand cdCommand(userInput);
			cdCommand.execute();
		}

		else if (invokeCommandName == "create") {
			CreateCommand createCommand(userInput);
			createCommand.execute();
		}

		else if (invokeCommandName == "open") {
			OpenCommand openCommand(userInput);
			openCommand.execute();
		}

		else if (invokeCommandName == "close") {
			CloseCommand closeCommand(userInput);
			closeCommand.execute();
		}

		else if (invokeCommandName == "write") {
			WriteCommand writeCommand(userInput);
			writeCommand.execute();
		}

		else if (invokeCommandName == "read") {
			ReadCommand readCommand(userInput);
			readCommand.execute();
		}

		else if (invokeCommandName == "delete") {
			DeleteCommand deleteCommand(userInput);
			deleteCommand.execute();
		}

		else if (invokeCommandName == "rmdir") {
			RmdirCommand rmdirCommand(userInput);
			rmdirCommand.execute();
		}

		else if (invokeCommandName == "sfs") {
			SfsCommand sfsCommand(userInput);
			sfsCommand.execute();
		}
		
		else {
			std::cout << Tip::NO_SUCH_COMMAND_ERROR << std::endl;
		}
	}
}

std::string CLIHandler::getInvokeCommandName(std::string userInput)
{
	std::string res;

	for (int i = 0; i < userInput.size(); i++) {
		if (userInput[i] == ' ') {
			return res;
		}

		res.push_back(userInput[i]);
	}

	return res;
}
