#include "InputHandler.h"
#include "Console.h"
#include "Controlls.h"
#include "DDEComponent.h"
#include "CommandExecutor.h"
#include <AntTweakBar/AntTweakBar.h>
#include <CEGUI/CEGUI.h>

using namespace DDEngine;

InputHandler::InputHandler() {
	clicked = false;
	focused = true;
}

InputHandler::~InputHandler() {

}

void DDEngine::InputHandler::onKeyTypedInternal(WPARAM wParam) {

	CommandExecutor& cmd = console->getExecutor();

	// F1 - solid
	KEY(VK_F1, wParam) {
		cmd.executeCommand("renderer.wireframe = false");
	}

	//F2 - wireframe
	KEY(VK_F2, wParam) {
		cmd.executeCommand("renderer.wireframe = true");
	}

	//F5 - listen all shaders to changes
	KEY(VK_F5, wParam) {
		cmd.executeCommand("shaders.listenAll");
	}

	//F6 - stop listening to changes
	KEY(VK_F6, wParam) {
		cmd.executeCommand("shaders.stop");
	}

	//F8 - reset camera
	KEY(VK_F8, wParam) {
		cmd.executeCommand("camera.reset");
	}

	//F9 - save camera
	KEY(VK_F9, wParam) {
		cmd.executeCommand("camera.save");
	}

	//F9 - save camera
	KEY(VK_F10, wParam) {
		cmd.executeCommand("camera.load");
	}

}

void InputHandler::handleAsyncInput() {

	if (GetAsyncKeyState(controlls->KEY_MOVE_FORWARD)) {
		controlls->moveForward();
	}

	if (GetAsyncKeyState(controlls->KEY_MOVE_BACKWARD)) {
		controlls->moveBackward();
	}

	if (GetAsyncKeyState(controlls->KEY_STRAFE_LEFT)) {
		controlls->strafeLeft();
	}

	if (GetAsyncKeyState(controlls->KEY_STRAFE_RIGHT)) {
		controlls->strafeRight();
	}

	// ON DRAGGED
	if(clicked) {
		POINT mouseCurrState;
		GetCursorPos(LPPOINT(&mouseCurrState));

		if(mousePrevState.x != mouseCurrState.x || mousePrevState.y != mouseCurrState.y) {
			controlls->mouseLook(mouseCurrState.x - mousePrevState.x, mouseCurrState.y - mousePrevState.y);
			mousePrevState = mouseCurrState;
		}
	}

	if(console->isHidden()) onKeyDownAsync();

}

bool InputHandler::handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {

	CEGUI::Key::Scan scan = (CEGUI::Key::Scan) virtualKeyToScanCode(wParam, lParam);
	POINT pos;

	if( TwEventWin(hWnd, msg, wParam, lParam) ) // send event message to AntTweakBar
        return true;

	switch (msg) {
		
		case WM_SIZE:
			RECT r;
			GetClientRect(hWnd, &r);
			TwWindowSize(r.right - r.left, r.bottom - r.top);
			break;

		case WM_KILLFOCUS:
			focused = false;
			break;

		case WM_SETFOCUS:
			focused = true;
			break;

		case WM_KEYDOWN:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(scan);
			break;

		case WM_KEYUP:
			if (console->isHidden()) {
				onKeyTypedInternal(wParam);
				onKeyTyped(wParam);
			}

			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(scan);
			break;

		case WM_CHAR:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectChar((char) wParam);
			break;

		case WM_MOUSEMOVE:
 			GetCursorPos(LPPOINT(&pos));
// 			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove((float) pos.x, (float) pos.y);
// 			CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition((float) pos.x, (float) pos.y);
			break;
			
		case WM_MOUSELEAVE:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseLeaves();
			break;

		case WM_LBUTTONDBLCLK:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDoubleClick(CEGUI::MouseButton::LeftButton);
			break;

		case WM_RBUTTONDBLCLK:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDoubleClick(CEGUI::MouseButton::RightButton);
			break;

		case WM_MBUTTONDBLCLK:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDoubleClick(CEGUI::MouseButton::MiddleButton);
			break;

		case WM_LBUTTONDOWN:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
			clicked = true;
			GetCursorPos(LPPOINT(&mousePrevState));
			break;
			
		case WM_RBUTTONDOWN:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::RightButton);
			break;

		case WM_MBUTTONDOWN:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
			break;

		case WM_LBUTTONUP:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonClick(CEGUI::MouseButton::LeftButton);
			clicked = false;
			break;

		case WM_RBUTTONUP:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::RightButton);
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonClick(CEGUI::MouseButton::RightButton);
			break;

		case WM_MBUTTONUP:
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::MiddleButton);
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonClick(CEGUI::MouseButton::MiddleButton);
			break;
			
		default:
			return false;
	
	}

	return true;
}

void InputHandler::setConsole(Console* console) {
	this->console = console;

	if(console) {
		controlls = &console->getDDEComponent().getControlls();
	}
}

bool InputHandler::hasFocus() {
	return focused;
}

void InputHandler::onKeyDownAsync() {

}

void InputHandler::onKeyTyped(WPARAM wParam) {

}