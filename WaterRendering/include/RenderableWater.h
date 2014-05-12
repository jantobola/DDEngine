#pragma once

#include <DDEngine.h>
#include <Grid.h>
#include <RenderToTexture.h>
#include <AntTweakBar/AntTweakBar.h>
#include "ConstantBuffers.h"

struct WaterProps_CB
{
	// float - 4 bytes
	// int - 4 bytes

	// c0
	float height = 0.084f;				// height of water drop
	float sizeX = 0;					// grid size x
	float sizeY = 0;					// grid size y
	int action = 0;						// 0 - computation; 1 - copy state 

	// c1
	float viscosity = 0.9948f;			// some constat for a momentum (representing something like viscosity)
	int waterDrop = 0;					// 0 - off; 1 - on
	int reset = 1;						// 0 - ignore; 1 - reset
	float heightOffset = -0.002f;		// water surface Y position + heightOffset = final Y position

	// c2
	DirectX::XMFLOAT2 refractiveTextureScale;
	DirectX::XMFLOAT2 pad;
};

class RenderableSkybox;
class RenderableTerrain;

class RenderableWater : public DDEngine::IRenderable {

	private:

		bool renderStep = false;
		bool oneStep = false;
		float time = 0.0f;

		TwBar* waterBar;

		RenderableSkybox* skybox;
		RenderableTerrain* terrain;

		DDEngine::ShaderResourceView* texture_bottom = nullptr;

		DDEngine::Dimension size;
		DDEngine::Grid waterSurface;
		DDEngine::Grid bottomSurface;

		DDEngine::SamplerState* samplerPointClamp = nullptr;			// point sampler
		DDEngine::SamplerState* samplerLinearClamp = nullptr;
		DDEngine::SamplerState* samplerLinearWrap = nullptr;

		DDEngine::RenderToTexture computeTexture_0;				// new values
		DDEngine::RenderToTexture computeTexture_1;				// storage (previous state)
		DDEngine::RenderToTexture reflectionTexture;			// texture with reflection map
		DDEngine::RenderToTexture refractionTexture;			// texture with refraction map

		Matrices_CB vsCB_0;
		Timer_CB vsCB_1;
		WaterProps_CB vsCB_2;
		Camera_CB vsCB_3;

		void setTweakBars();

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
		
		void setSkybox(RenderableSkybox* skybox) { this->skybox = skybox; }
		void setTerrain(RenderableTerrain* terrain) { this->terrain = terrain; }

};