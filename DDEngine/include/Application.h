#pragma once

#include <windows.h>

namespace DDEngine
{

class Window;
class DDEComponent;
class InputHandler;

class Application {

	private:
    
		HINSTANCE hInstance;
		DDEComponent& component;
		Window* window;
		InputHandler* input;

	public:

		Application(HINSTANCE hInstance, DDEComponent& component);
		~Application();

		int run();
		void setKeyListener(InputHandler* input);

		static int bootstrap(HINSTANCE hInstance, DDEComponent& component);

};
}