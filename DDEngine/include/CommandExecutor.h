#pragma once

#include "AbstractCommandExecutor.h"
#include <functional>

namespace DDEngine
{
class CommandExecutor : public AbstractCommandExecutor {
	
	private:
		std::function<void(const std::vector<std::string>&)> _delegate;

	public:
		CommandExecutor(DDEComponent& component) : AbstractCommandExecutor(component) { };
		~CommandExecutor() { };

		virtual void executeCommand(std::string command) override;
		virtual void delegate(std::function<void(const std::vector<std::string>&)> func);

};
}