#include "Application.h"

#include "Window.h"
#include "DDEComponent.h"
#include "InputHandler.h"
#include "Console.h"
#include "HUDRenderer.h"
#include <CommandExecutor.h>

using namespace DDEngine;

Application::Application( HINSTANCE hInstance, DDEComponent& component ) : 
	hInstance( hInstance ), 
	component( component ) {
	
	Config& cfg = component.getConfig();

	// WINAPI part - create window
	window = new Window(hInstance);
	window->setTitle(cfg.CFG_WINDOW_TITLE.c_str());
}

Application::~Application() {
	delete window;
}

int Application::run() {

    MSG msg = { 0 };

	Config& cfg = component.getConfig();
	
	window->initWindow(cfg.CFG_SCREEN_WIDTH, cfg.CFG_SCREEN_HEIGHT);

	// build DDEComponent using D3DRenderer
	component.setHInstance(hInstance);
	component.setMultiSamplingValue(cfg.MSAA);

	component.initDevice(window->getHwnd());
	window->show();

	component.getHUD().loadingScreen("Initializing...");
	component.compose();

	// create console for a given DDEComponent
	Console console = Console(component);

	if(input) {
		input->setConsole(&console);
		window->setHandler(input);
	}

	component.setCommandExecutor(console.getExecutor());
	component.buildAll();

	console.getExecutor().executeBatch(cfg.getRenderConfig());
	component.getHUD().clearNotifications();
	component.getTimer().init();

	while ((msg.message != WM_QUIT) && component.isRunning()) {
       
		component.getTimer().start();
		component.getCamera().update();

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        } else {

			if (input) {
				if (console.isHidden() && input->hasFocus()) {
					input->handleAsyncInput();
				}
			}

            component.renderFrame();
       }
    }

	return (int) msg.wParam;
}

void Application::setKeyListener( InputHandler* input ) {
	this->input = input;
}

int Application::bootstrap( HINSTANCE hInstance, DDEComponent& component ) {
	Application application = Application(hInstance, component);
	InputHandler defaultListener = InputHandler();

	application.setKeyListener(&defaultListener);
	int result = application.run();

	return result;
}

Window& DDEngine::Application::getWindow() {
	return *window;
}
