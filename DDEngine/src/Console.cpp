#include "Console.h"
#include "CommandExecutor.h"
#include <CEGUI/CEGUI.h>

using namespace DDEngine;
using namespace std;

Console::Console(DDEComponent& component) : component(component) {
	hidden = true;
	create();
}

Console::~Console() {
	delete cmdExecutor;
}

void Console::toggleConsole() {
	if (hidden) {
		cmd->show();
		hidden = false;
		cmd->activate();
	} else {
		cmd->hide();
		hidden = true;
		cmd->deactivate();
	}

}

void Console::create() {
	using namespace CEGUI;

	WindowManager& windowManager = WindowManager::getSingleton();
	CEGUI::Window* rootWindow = System::getSingleton().getDefaultGUIContext().getRootWindow();
	rootWindow->subscribeEvent(CEGUI::Window::EventKeyUp, Event::Subscriber(&Console::invokeGlobalKeyUp, this));
	rootWindow->subscribeEvent(CEGUI::Window::EventKeyDown, Event::Subscriber(&Console::invokeGlobalKeyDown, this));

	cmd = static_cast<Editbox*>( windowManager.createWindow( "AlfiskoSkin/Editbox", "consoleEditbox" ));

	rootWindow->addChild( cmd );
	cmd->setPosition(UVector2( UDim( 0.0f, 0 ), UDim( 0.955f, 0 ) ));
	cmd->setSize(USize( UDim(1.0f, 0), UDim(0.045f, 0) ));
	cmd->setAlpha(1.0f);
	cmd->setReadOnly(false);
	cmd->hide();

	cmd->subscribeEvent(Editbox::EventTextAccepted, Event::Subscriber(&Console::sendCommand, this));
	cmd->subscribeEvent(Editbox::EventKeyUp, Event::Subscriber(&Console::invokeKeyUp, this));
	rootWindow->activate();

	cmdExecutor = new CommandExecutor(component);
}

bool Console::sendCommand(const CEGUI::EventArgs &e) {
	cmdExecutor->executeCommand(cmd->getText().c_str());
	cmd->setText("");
	return true;
}

bool Console::invokeKeyUp(const CEGUI::EventArgs &e) {
	const CEGUI::KeyEventArgs* args = static_cast<const CEGUI::KeyEventArgs*>(&e);

	if (args->scancode == CEGUI::Key::ArrowUp) {
		string prev = cmdExecutor->previousCommand();
		cmd->setText(prev);
		cmd->setCaretIndex(prev.length());
		return true;
	}

	if (args->scancode == CEGUI::Key::ArrowDown) {
		string next = cmdExecutor->nextCommand();
		cmd->setText(next);
		cmd->setCaretIndex(next.length());
		return true;
	}

	return false;
}

bool Console::invokeKeyDown(const CEGUI::EventArgs &e) {
	const CEGUI::KeyEventArgs* args = static_cast<const CEGUI::KeyEventArgs*>(&e);

	return false;
}

bool Console::invokeGlobalKeyUp( const CEGUI::EventArgs &e ) {
	const CEGUI::KeyEventArgs* args = static_cast<const CEGUI::KeyEventArgs*>(&e);

	if (args->scancode == CEGUI::Key::Grave) {
		if(!hidden) {
			CEGUI::String s = cmd->getText();
			s = s.substr(0, s.length() - 1);
			cmd->setText(s);
		}
		toggleConsole();
		return true;
	}

	return false;
}

bool Console::invokeGlobalKeyDown(const CEGUI::EventArgs &e) {
	const CEGUI::KeyEventArgs* args = static_cast<const CEGUI::KeyEventArgs*>(&e);

	return false;
}

bool Console::isHidden() {
	return this->hidden;
}

CommandExecutor& Console::getExecutor() {
	return *cmdExecutor;
}

DDEComponent& Console::getDDEComponent() {
	return component;
}
