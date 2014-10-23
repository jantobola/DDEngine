#include "DDEComponent.h"
#include "DDEUtils.h"
#include "Controlls.h"
#include "ResourceProvider.h"
#include "ScenesManager.h"
#include "HUDRenderer.h"
#include "GUIRenderer.h"
#include <AntTweakBar/AntTweakBar.h>

using namespace DDEngine;
using namespace std;

DDEComponent::DDEComponent(std::string configPath /* = DDE_CONFIG_PATH */) {
	config.setConfigPath(configPath);
	config.load();
	camera.setSavePath(FileUtils::splitFilename(configPath));
	camera.loadCamera();
}

DDEComponent::~DDEComponent() {
	this->cleanUp();

	delete resources;
	delete gui;
	delete hud;
	delete scenes;
	delete controlls;
}

void DDEngine::DDEComponent::initDevice(HWND hWnd) {

	// Initialization of D3DRenderer
	if (FAILED(this->initialize(hWnd))) {
		// initialization error code
		PostQuitMessage(-1);
	}

	hud = new HUDRenderer(config, Ctx, timer);
}

void DDEngine::DDEComponent::compileShaders()
{
	// Default relative path from executable
	wstring	path = L"shaders/";

	// If debug configuration is active, use working shaders directory.
	// You can recompile these shaders at runtime.
	#ifdef DEBUG
		path = L"../../../src/shaders/";
	#endif

	for (ShaderConfig shader : config.shaders) {
		switch (shader.type)
		{
		case VS:
			shaders->addVertexShader(shader.name, path + StringUtils::toWstring(shader.file), shader.model, shader.entry);
			break;

		case PS:
			shaders->addPixelShader(shader.name, path + StringUtils::toWstring(shader.file), shader.model, shader.entry);
			break;

		default:
			Win32Utils::showMessage("Shader Compilation Error", "Unknown type of shader.");
			break;
		}
	}
}

void DDEComponent::compose() {
	
	controlls	= new Controls(camera, timer);
	scenes		= new ScenesManager(config, Ctx);
	gui			= new GUIRenderer(config, Ctx);
	resources	= new ResourceProvider(config, Ctx);
	shaders		= &resources->getShaderHolder();
	
	resources->load();
	Ctx.setShaderHolder(shaders);
	hud->addText("fps", "", 10, 10);

	TwInit(TW_DIRECT3D11, Ctx.device);
	TwWindowSize(screenDimension.WIDTH, screenDimension.HEIGHT);
	TwDefine(" TW_HELP visible=false ");
	TwDefine(" GLOBAL fontstyle=fixed ");
	TwDefine(" GLOBAL fontresizable=false ");

}

void DDEComponent::buildAll() {
	hud->loadingScreen("Compiling shaders...");
	compileShaders();

	hud->loadingScreen("Loading scene...");
	create();
	scenes->create();
}

void DDEComponent::onRender() {
	resources->getShaderHolder().realTimeCompile();
	updateHUD();
	
	render();
	scenes->render(getRenderPackage());
	
	hud->render();
	gui->render();
	TwDraw();

	resources->getShaderHolder().refreshShaders();
}

void DDEComponent::updateHUD() {
	int curr_fps = timer.getFPS();
	hud->update("fps", string("FPS: ") + to_string(curr_fps));
}

void DDEComponent::setHInstance( HINSTANCE hInstance ) {
	this->hInstance = hInstance;
}

HINSTANCE DDEComponent::getHInstance() {
	return hInstance;
}

bool DDEComponent::isRunning() {
	return running;
}

void DDEComponent::quit() {
	running = false;
	PostQuitMessage(0);
}

void DDEComponent::cleanUp() {
	if(Ctx.swapChain) Ctx.swapChain->SetFullscreenState(false, NULL);
	TwTerminate();
}

DDERenderPackage DDEComponent::getRenderPackage() {
	return DDERenderPackage(Ctx, *resources, resources->getShaderHolder(), *hud, camera, timer, config);
}