#pragma once

#include <DDEngine.h>
#include <Grid.h>
#include <RenderToTexture.h>
#include <AntTweakBar/AntTweakBar.h>
#include "ConstantBuffers.h"

struct WaterProps_CB
{
	WaterProps_CB() {

	}

	// float - 4 bytes
	// int - 4 bytes

	// align
	float height{ 0.024f };
	float sizeX{ 0 };
	float sizeY{ 0 };
	int action{ 0 }; // 0 - computation; 1 - copy state 
	
	// align
	float viscosity{ 0.9998f }; // some constat for a slower or faster attenuation (representing something like viscosity)
	int waterDrop{ 0 }; // 0 - off; 1 - on
	int reset{ 1 }; // 0 - ignore; 1 - reset
	float pad;
};

class RenderableWater : public DDEngine::IRenderable {

	private:

		bool renderStep = false;
		bool oneStep = false;
		float time = 0.0f;

		TwBar* waterBar;

		DDEngine::ShaderResourceView* texture_bottom = nullptr;

		DDEngine::Dimension size;
		DDEngine::Grid waterSurface;
		DDEngine::Grid bottomSurface;

		DDEngine::SamplerState* waterSampler = nullptr; // point sampler
		DDEngine::SamplerState* linearSampler = nullptr;

		DDEngine::RenderToTexture computeTexture_0; // new values
		DDEngine::RenderToTexture computeTexture_1; // storage (previous state)

		Matrices_CB vsCB_0;
		Timer_CB vsCB_1;
		WaterProps_CB vsCB_2;

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
		
};