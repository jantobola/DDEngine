#pragma once

#include <DDEngine.h>
#include <RenderableSkybox.h>
#include <AntTweakBar/AntTweakBar.h>
#include "ConstantBuffers.h"

class RenderableWater;
class RenderableTerrain;

class CustomRenderer : public DDEngine::DDEComponent {
	
	private:
		
		::Light_CB light;
		TwBar* tweakBar = nullptr;
				
		Matrices_CB mat;

		void cleanUp();
		void initShaders();
		void setTweakBars();

	public:

		CustomRenderer(std::string configPath = DDE_CONFIG_PATH);
		~CustomRenderer();

		RenderableTerrain* terrain = nullptr;
		DDEngine::RenderableSkybox* skybox = nullptr;
		RenderableWater* water = nullptr;

		virtual void create() override;

	protected:

		virtual void render() override;
}; 