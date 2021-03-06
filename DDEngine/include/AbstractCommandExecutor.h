#pragma once

#include <string>
#include <vector>

#define CMD_DELIMITER '='
#define ARG_DELIMITER ','

#define CMD_HISTORY_SIZE 20

namespace DDEngine
{

class DDEComponent;

class AbstractCommandExecutor {

	private:

		std::string lastCommand;

	public:

		AbstractCommandExecutor(DDEComponent& component);
		
		std::string previousCommand();
		std::string nextCommand();

		virtual void executeCommand(std::string command) = 0;

		void executeBatch(std::vector<std::string> batch);

	protected:

		~AbstractCommandExecutor();

		std::vector<std::string> commandList;
		int cmdListIndex;
		int historyIndex;
		bool executed;

		DDEComponent& component;

		void parseCommand(std::string cmd, std::vector<std::string>& tokens);
		void manageHistory(std::string cmd);

};
}