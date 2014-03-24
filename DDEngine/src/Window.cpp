#include "Window.h"

using namespace DDEngine;

Window::Window( HINSTANCE hInstance ) : hInstance( hInstance ) {

	title = L"DDEngine Application";
	wndClass = L"DDEngineWindow";
	shown = false;
	handler = NULL;
}

Window::~Window() {

}

HRESULT Window::initWindow( int width, int height ) {

	this->width = width;
	this->height = height;

	// Register class
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = staticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = wndClass;
	wcex.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if( !RegisterClassEx( &wcex ) ) {
		return E_FAIL;	
	}

	// Create window
	RECT rc = { 0, 0, this->width, this->height };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	hwnd = CreateWindow( wndClass, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
						 rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, this );

	if( !hwnd ) {
		return E_FAIL;
	}

#if defined( DEBUG ) && defined( PRECOMPILED_SHADERS )
	//SetWindowPos(hwnd, HWND_TOP, 2467, 241, 0, 0, SWP_NOSIZE);
#endif

	return S_OK;
}

LRESULT CALLBACK Window::staticWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	Window* window = NULL;

	// Get pointer to window
	if( msg == WM_CREATE ) {
		
		window = (Window*) ( (LPCREATESTRUCT) lParam )->lpCreateParams;
		SetWindowLongPtr( hwnd, GWL_USERDATA, (LONG_PTR) window );
		
	} else {
			
		window = (Window*) GetWindowLongPtr( hwnd, GWL_USERDATA );

		if( !window ) {
			return DefWindowProc( hwnd, msg, wParam, lParam );
		} 
	}

	if(window->handler) {

		return window->handler->WndProc( hwnd, msg, wParam, lParam );

	} else {

		if(msg == WM_DESTROY) {
			PostQuitMessage(0);
		}

		return DefWindowProc( hwnd, msg, wParam, lParam );
	} 
}

void Window::show() {
	ShowWindow( hwnd, SW_SHOW );
	shown = true;
}

void Window::hide() {
	ShowWindow( hwnd, SW_HIDE );
	shown = false;
}

void Window::setHandler( WndHandler* handler ) {
	this->handler = handler;
}

void Window::setTitle( LPCTSTR title ) {
	this->title = title;
}

HWND Window::getHwnd() {
	return hwnd;
}