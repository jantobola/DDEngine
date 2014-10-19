#include "AbstractCommandExecutor.h"
#include "CommandParser.h"

using namespace DDEngine;

AbstractCommandExecutor::AbstractCommandExecutor(DDEComponent& component) : component(component) {
	cmdListIndex = 0;
	historyIndex = 0;
	executed = true;
	commandList.resize(CMD_HISTORY_SIZE);

	for (int i = 0; i < CMD_HISTORY_SIZE; i++) {
		commandList.at(i).reserve(258);
	}
}

AbstractCommandExecutor::~AbstractCommandExecutor() {

}

void AbstractCommandExecutor::parseCommand(std::string cmd, std::vector<std::string>& tokens) {
	std::string toParse = cmd.c_str();
	CommandParser::parse(&tokens, CMD_DELIMITER, ARG_DELIMITER, toParse);
}

void AbstractCommandExecutor::manageHistory(std::string cmd) {

	if(lastCommand == cmd) return;
	commandList.at(cmdListIndex) = cmd;
	lastCommand = cmd;

	cmdListIndex++;
	historyIndex = cmdListIndex;

	if(cmdListIndex == CMD_HISTORY_SIZE) {
		cmdListIndex = 0;
	}
}

std::string AbstractCommandExecutor::previousCommand() {
	historyIndex--;

	if(historyIndex == -1) {
		historyIndex = CMD_HISTORY_SIZE - 1;
	}

	return commandList.at(historyIndex);
}

std::string AbstractCommandExecutor::nextCommand() {
	historyIndex++;

	if(historyIndex == CMD_HISTORY_SIZE) {
		historyIndex = 0;
	}

	return commandList.at(historyIndex);
}

void AbstractCommandExecutor::executeBatch(std::vector<std::string> batch) {
	for (size_t i = 0; i < batch.size(); i++) {
		executeCommand(batch.at(i));
	}
}
