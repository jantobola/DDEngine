#pragma once

#include <DDEngine.h>
#include <Grid.h>
#include <RenderToTexture.h>
#include <AntTweakBar/AntTweakBar.h>
#include "ConstantBuffers.h"

// to tweak bar
#define TERRAIN_GRID_SIZE 512, 512
#define TERRAIN_SHADERS "VS_TerrainVDisplacement", "PS_BasicLightMesh", "POS2"

struct Terrain_CB {

	//c0
	DirectX::XMFLOAT2 textureSize;
	float elevationFactor = 0.2f;
	float scaleFactor = 1;

	//c1
	float textureScaleFactor = 20;
	DirectX::XMFLOAT3 pad2;
};

class RenderableTerrain : public DDEngine::IRenderable {

	public:
	
		DDEngine::Grid terrain;
		Matrices_CB vsCB_0;
		Terrain_CB vsCB_1;

		DDEngine::ShaderResourceView* grassTexture = nullptr;
		DDEngine::ShaderResourceView* dustTexture = nullptr;
		DDEngine::ShaderResourceView* terrainTexture = nullptr;
		DDEngine::SamplerState* samplerLinearClamp = nullptr;
		DDEngine::SamplerState* samplerLinearWrap = nullptr;
		DDEngine::SamplerState* samplerAnisotropicWrap = nullptr;

		DDEngine::RenderToTexture perspectiveView;

		RenderableTerrain(DDEngine::DDERenderPackage pkg) : DDEngine::IRenderable(pkg) { }
		~RenderableTerrain();
	
		virtual void create() override;
		virtual void render() override;

		void setProceduralGeneration(bool procedural);

	protected:
		
		bool procedural = false;
		DDEngine::RenderToTexture proceduralTerrainTexture;

		TwBar* terrainBar = nullptr;

	private:

		void setTweakBars();
};
