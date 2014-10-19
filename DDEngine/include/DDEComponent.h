#pragma once

#define NOMINMAX
#define TW_NO_LIB_PRAGMA

#include "Config.h"
#include "D3DRenderer.h"
#include "DDERenderPackage.h"
#include "CommandExecutor.h"

namespace DDEngine
{

class ScenesManager;
class GUIRenderer;
class Controls;
class HUDRenderer;

class DDEComponent : public D3DRenderer {

	private:

		HINSTANCE hInstance;
		bool running = true;
		bool renderStep = false;

		CommandExecutor* commandExecutor;

		void compileShaders();
		void updateHUD();
		void cleanUp();
		
	public:

		DDEComponent(std::string configPath = DDE_CONFIG_PATH);
		virtual ~DDEComponent();
	
		Config& getConfig() { return config; }
		ScenesManager& getScenesManager() { return *scenes; }
		GUIRenderer& getGUI() { return *gui; }
		HUDRenderer& getHUD() { return *hud; }
		Controls& getControlls() { return *controlls; }
		ResourceProvider& getResources() { return *resources; }

		DDERenderPackage getRenderPackage();

		void setCommandExecutor(CommandExecutor& executor) { this->commandExecutor = &executor; }
		CommandExecutor& getCommandExecutor() { return *commandExecutor; }

		void initDevice(HWND hWnd);
		void compose();
		void buildAll();

		void setHInstance(HINSTANCE hInstance);
		HINSTANCE getHInstance();

		bool isRunning();
		void quit();

	protected:

		Config config;
		
		ScenesManager* scenes = nullptr;
		GUIRenderer* gui = nullptr;
		HUDRenderer* hud = nullptr;
		Controls* controlls = nullptr;
		ResourceProvider* resources = nullptr;

		// do not delete
		ShaderHolder* shaders;
		
		virtual void onRender() override;

		virtual void render() = 0;
		virtual void create() = 0;

};
}