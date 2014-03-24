#pragma once

namespace CEGUI
{
class Editbox;
class EventArgs;
}

namespace DDEngine
{

class DDEComponent;
class CommandExecutor;

class Console {

	private:
		CommandExecutor* cmdExecutor;
		CEGUI::Editbox* cmd;
		bool hidden;

		bool sendCommand(const CEGUI::EventArgs &e);
		bool invokeKeyDown(const CEGUI::EventArgs &e);
		bool invokeKeyUp(const CEGUI::EventArgs &e);
		bool invokeGlobalKeyUp(const CEGUI::EventArgs &e);
		bool invokeGlobalKeyDown(const CEGUI::EventArgs &e);

		DDEComponent& component;
		void create();

	public:
		Console(DDEComponent& component);
		~Console();

		void toggleConsole();
		bool isHidden();
		CommandExecutor& getExecutor();
		DDEComponent& getDDEComponent();
};
}