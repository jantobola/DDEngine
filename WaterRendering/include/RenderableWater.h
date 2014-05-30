#pragma once

#include <DDEngine.h>
#include <RenderableSkybox.h>
#include <Grid.h>
#include <RenderToTexture.h>
#include <AntTweakBar/AntTweakBar.h>
#include "ConstantBuffers.h"

struct WaterProps_CB
{
	// float - 4 bytes
	// int - 4 bytes

	// c0
	float height = 0.0012f;				// height of water drop
	float sizeX = 0;					// grid size x
	float sizeY = 0;					// grid size y
	int action = 0;						// 0 - computation; 1 - copy state 

	// c1
	float viscosity = 0.9975f;			// some constat for a momentum (representing something like viscosity)
	int waterDrop = 0;					// 0 - off; 1 - on
	int reset = 1;						// 0 - ignore; 1 - reset
	float heightOffset = -0.001f;		// water surface Y position + heightOffset = final Y position

};

class RenderableSkybox;
class RenderableTerrain;

class RenderableWater : public DDEngine::IRenderable {

	private:

		bool renderStep = false;
		bool oneStep = false;
		float time = 0.0f;

		TwBar* waterBar;

		DDEngine::RenderableSkybox* skybox;
		RenderableTerrain* terrain;

		DDEngine::Dimension size;
		DDEngine::Grid waterSurface;

		DDEngine::SamplerState* samplerPointClamp = nullptr;			// point sampler
		DDEngine::SamplerState* samplerLinearClamp = nullptr;
		DDEngine::SamplerState* samplerLinearWrap = nullptr;

		DDEngine::RenderToTexture computeTexture_0;				// new values
		DDEngine::RenderToTexture computeTexture_1;				// storage (previous state)
		DDEngine::RenderToTexture computeTexture_2;
		DDEngine::RenderToTexture computeTexture_3;

		Matrices_CB vsCB_0;
		Timer_CB vsCB_1;
		WaterProps_CB vsCB_2;
		Camera_CB vsCB_3;

		void setTweakBars();
		void setAction(int action);
		bool isStep();

	public:

		RenderableWater(DDEngine::DDERenderPackage pkg) : DDEngine::IRenderable(pkg) { }
		~RenderableWater();

		void setWaterDrop(bool isOn);
		void resetSurface(bool reset);
		void step();
		void setStepping(bool stepping);
	
		virtual void create() override;
		virtual void render() override;

		void setSize(int width, int height);
		DDEngine::Dimension getSize();
		
		void setSkybox(DDEngine::RenderableSkybox* skybox) { this->skybox = skybox; }
		void setTerrain(RenderableTerrain* terrain) { this->terrain = terrain; }

};