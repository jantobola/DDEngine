#include "Window.h"
#include "DLLResourceLoader.h"

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

	if(this->icon == NULL) {
		wcex.hIcon = DLLResourceLoader::loadWindowIcon();
		wcex.hIconSm = DLLResourceLoader::loadWindowIcon();	
	} else {
		wcex.hIcon = this->icon;
		wcex.hIconSm = this->icon;
	}

	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = wndClass;

	if( !RegisterClassEx( &wcex ) ) {
		return E_FAIL;	
	}

	// Create window
	RECT rc = { 0, 0, this->width, this->height };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	int posX = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;

	hwnd = CreateWindow(wndClass, title, WS_OVERLAPPEDWINDOW, posX, 
		posY, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, this );

	if( !hwnd ) {
		return E_FAIL;
	}

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

void DDEngine::Window::setIcon(HICON icon) {
	this->icon = icon;
}
