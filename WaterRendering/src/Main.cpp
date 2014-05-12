#include "CustomRenderer.h"
#include "KeyListener.h"
#include <windows.h>
#include <Window.h>

#if defined(DEBUG)
	#define CONFIG_PATH "../../../data/config.cfg"
#else
	#define CONFIG_PATH "config.cfg"
#endif

using namespace DDEngine;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int	nCmdShow) {

	CustomRenderer renderer(CONFIG_PATH);
	KeyListener listener(&renderer);
	Application app(hInstance, renderer);

	app.getWindow();
	app.setKeyListener(&listener);
	return app.run();
}