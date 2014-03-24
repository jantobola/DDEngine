#pragma once

#include <windows.h>

namespace DDEngine
{
class WndHandler {

	public:
		WndHandler();
		virtual ~WndHandler();

		// The window procedure, as a member function
		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
		
	protected:
		virtual bool handle( HWND, UINT, WPARAM, LPARAM ) { return false; }
		UINT virtualKeyToScanCode(WPARAM wParam, LPARAM lParam);

};
}