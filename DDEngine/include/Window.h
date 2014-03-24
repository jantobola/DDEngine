#pragma once

#include "WndHandler.h"

namespace DDEngine
{
class Window {

	private:
		bool			shown;
		LPCTSTR			wndClass;
		LPCTSTR			title;
		HWND			hwnd;
		HINSTANCE		hInstance;
		WndHandler*		handler;
	
	public:
		int	width;
		int	height;
	
		Window( HINSTANCE hInstance );
		~Window();

		HRESULT initWindow( int width, int height );
		void show();
		void hide();
	
		void setHandler(WndHandler* handler);
		void setTitle(LPCTSTR title);

		HWND getHwnd();

		static LRESULT CALLBACK staticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};
}