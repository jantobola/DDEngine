#include "WndHandler.h"
#include "Window.h"

using namespace DDEngine;

WndHandler::WndHandler() {

}

WndHandler::~WndHandler() {

}

LRESULT CALLBACK WndHandler::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
		case WM_DESTROY:
			// Post WM_QUIT to message queue, stopping program
			PostQuitMessage(0);
			break;

		default:
			if( !handle( hwnd, msg, wp, lp ) ) {
				return DefWindowProc( hwnd, msg, wp, lp );
			}
		}

	return 0;
}

UINT WndHandler::virtualKeyToScanCode(WPARAM wParam, LPARAM lParam) {
	if(HIWORD(lParam) & 0x0F00) {
		UINT scancode = MapVirtualKey(wParam, 0); 
		return scancode | 0x80;
	}
	else {
		return HIWORD(lParam) & 0x00FF;
	}
}