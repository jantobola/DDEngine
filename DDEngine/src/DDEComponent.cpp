#include "DDEComponent.h"

#include "Controlls.h"
#include "ResourceProvider.h"
#include "ObjectManager.h"
#include "HUDRenderer.h"
#include "GUIRenderer.h"
#include <AntTweakBar/AntTweakBar.h>

using namespace DDEngine;
using namespace std;

DDEComponent::DDEComponent(std::string configPath /* = DDE_CONFIG_PATH */) {
	config.setConfigPath(configPath);
	config.load();
}

DDEComponent::~DDEComponent() {
	this->cleanUp();

	delete resources;
	delete gui;
	delete hud;
	delete objects;
	delete controlls;
}

void DDEComponent::compose(HWND hWnd) {
	
	// Initialization of D3DRenderer
	if (FAILED(this->initialize(hWnd))) {
		// initialization error code
		PostQuitMessage(-1);
	}

	controlls	= new Controlls(camera, timer);
	objects		= new ObjectManager(config, Ctx);
	hud			= new HUDRenderer(config, Ctx);
	gui			= new GUIRenderer(config, Ctx);
	resources	= new ResourceProvider(config, Ctx);
	shaders		= &resources->getShaderHolder();

	resources->load();
	hud->addText("fps", "", 10, 10);

	TwInit(TW_DIRECT3D11, Ctx.device);
	TwWindowSize(screenDimension.WIDTH, screenDimension.HEIGHT);
	TwDefine(" TW_HELP visible=false ");
	
	create();
	objects->create();
}

void DDEComponent::onRender() {
	resources->getShaderHolder().realTimeCompile();
	updateHUD();
	
	render();
	objects->render();
	
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

Config& DDEComponent::getConfig() {
	return config;
}

ObjectManager* DDEComponent::getObjectManager() {
	return objects;
}

GUIRenderer* DDEComponent::getGUI() {
	return gui;
}

HUDRenderer* DDEComponent::getHUD() {
	return hud;
}

Controlls* DDEComponent::getControlls() {
	return controlls;
}

ResourceProvider* DDEComponent::getResources() {
	return resources;
}

void DDEComponent::cleanUp() {
	if(Ctx.swapChain) Ctx.swapChain->SetFullscreenState(false, NULL);
	TwTerminate();
}

DDERenderPackage DDEComponent::getRenderPackage() {
	return DDERenderPackage(Ctx, *resources, resources->getShaderHolder(), *hud, camera, timer, config);
}