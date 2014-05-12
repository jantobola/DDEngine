#pragma once
#define NOMINMAX

#include "WndHandler.h"

namespace DDEngine
{

class Console;
class Controls;

class InputHandler : public WndHandler {

	private:
		
		bool clicked;
		bool focused;
		POINT mousePrevState;

	public:

		InputHandler();
		virtual ~InputHandler();

		void handleAsyncInput();
		bool hasFocus();
		void setConsole(Console* console);

	protected:

		Console* console;
		Controls* controlls;

		virtual bool handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		virtual void onKeyDownAsync();
		virtual void onKeyTyped(WPARAM wParam);
	
};
}

#define ASYNC_KEY(X) if(GetAsyncKeyState(X))
#define KEY(K,W) if(W == K)