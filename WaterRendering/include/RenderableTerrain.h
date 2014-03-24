#pragma once

#include <DDEngine.h>
#include <Grid.h>
#include "ConstantBuffers.h"

#define TERRAIN_GRID_SIZE 1000, 1000
#define TERRAIN_SIZE 3000, 3000, 50
#define TERRAIN_SHADERS "TerrainVS", "MeshPS", "POS4_TEX"

struct Terrain_CB {

	Terrain_CB() {
		ZeroMemory(this, sizeof(Terrain_CB));
	}

	DirectX::XMFLOAT3 customSize;
	float pad;
	DirectX::XMFLOAT2 textureSize;
	DirectX::XMFLOAT2 pad2;
};

class RenderableTerrain : public DDEngine::IRenderable {

	private:
		
		DDEngine::Grid terrain;
		Matrices_CB vsCB_0;
		Terrain_CB psCB_1;

		ID3D11ShaderResourceView* grassTexture{ NULL };
		ID3D11ShaderResourceView* terrainTexture{ NULL };
		ID3D11SamplerState* terrainSampler{ NULL };

	public:

		RenderableTerrain(DDEngine::DDERenderPackage pkg) : DDEngine::IRenderable(pkg) { }
		~RenderableTerrain();
	
		virtual void create() override;
		virtual void render() override;

	protected:
	
};
