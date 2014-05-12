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
		Window* window = nullptr;
		InputHandler* input = nullptr;

	public:

		Application(HINSTANCE hInstance, DDEComponent& component);
		~Application();

		int run();
		void setKeyListener(InputHandler* input);
		Window& getWindow();

		static int bootstrap(HINSTANCE hInstance, DDEComponent& component);

};
}