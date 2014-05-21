#pragma once

#define NOMINMAX
#define TW_NO_LIB_PRAGMA

#include "Config.h"
#include "D3DRenderer.h"
#include "DDERenderPackage.h"

namespace DDEngine
{

class ObjectManager;
class GUIRenderer;
class Controls;
class HUDRenderer;

class DDEComponent : public D3DRenderer {

	private:

		HINSTANCE hInstance;
		bool running = true;
		bool renderStep = false;

		void updateHUD();
		void cleanUp();
		
	public:

		DDEComponent(std::string configPath = DDE_CONFIG_PATH);
		virtual ~DDEComponent();
	
		Config& getConfig();
		// should return reference instead of pointers!
		ObjectManager* getObjectManager();
		GUIRenderer* getGUI();
		HUDRenderer* getHUD();
		Controls* getControlls();
		ResourceProvider* getResources();

		DDERenderPackage getRenderPackage();

		void initDevice(HWND hWnd);
		void compose();

		void setHInstance(HINSTANCE hInstance);
		HINSTANCE getHInstance();

		bool isRunning();
		void quit();

	protected:

		Config config;
		
		ObjectManager* objects = nullptr;
		GUIRenderer* gui = nullptr;
		HUDRenderer* hud = nullptr;
		Controls* controlls = nullptr;
		ResourceProvider* resources = nullptr;

		// do not delete
		ShaderHolder*		shaders;
		
		virtual void onRender() override;

		virtual void render() = 0;
		virtual void create() = 0;
		
};
}