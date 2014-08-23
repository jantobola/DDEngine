#include "CustomRenderer.h"
#include "KeyListener.h"
#include <windows.h>
#include <Window.h>

#include <DDERenderPackage.h>
#include <RenderContext.h>

#if defined(DEBUG)
	#define CONFIG_PATH "../../../data/config.cfg"
#else
	#define CONFIG_PATH "config.cfg"
#endif

using namespace DDEngine;

ID3D11Debug* dbg;

int start(HINSTANCE hInstance) {
	CustomRenderer renderer(CONFIG_PATH);

	KeyListener listener(&renderer);
	Application app(hInstance, renderer);

	app.setKeyListener(&listener);
	int ret = app.run();

#if defined(DEBUG)
	renderer.getRenderPackage().renderContext.device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&dbg));
#endif
	
	return ret;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int	nCmdShow) {
	int ret = start(hInstance);

#if defined(DEBUG)
	dbg->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
	dbg->Release();
#endif

	return ret;
}