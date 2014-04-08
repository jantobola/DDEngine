#include "CustomRenderer.h"
#include "KeyListener.h"
#include <windows.h>

#if defined( DEBUG ) || defined( _DEBUG )
	#define CONFIG_PATH "../../../data/config.cfg"
#else
	#define CONFIG_PATH "config.cfg"
#endif

using namespace DDEngine;

//--------------------------------------------------------------------------------------
// Entry point to the program.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE	hInstance, 
					HINSTANCE	hPrevInstance, 
					LPWSTR		lpCmdLine, 
					int			nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );
	UNREFERENCED_PARAMETER( nCmdShow );

	CustomRenderer renderer(CONFIG_PATH);
	renderer.setMultiSamplingValue(2);

	KeyListener listener(&renderer);

	Application app(hInstance, renderer);
	app.setKeyListener(&listener);
	return app.run();
}