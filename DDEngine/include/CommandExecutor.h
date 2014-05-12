#pragma once

#include "AbstractCommandExecutor.h"

namespace DDEngine
{
class CommandExecutor : public AbstractCommandExecutor {
	
	public:
		CommandExecutor(DDEComponent& component) : AbstractCommandExecutor(component) { };
		~CommandExecutor() { };

		virtual void executeCommand(std::string command) override;
		virtual void executeCommand(std::string command, std::function<void()>&& funct) override;

};
}