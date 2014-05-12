#include "RenderableTerrain.h"
#include <d3dx11.h>
#include <DirectXTK/DDSTextureLoader.h>

using namespace DDEngine;
using namespace std;
using namespace DirectX;

RenderableTerrain::~RenderableTerrain() {
	terrain.releaseBuffers();
	
	if(procedural) {
		terrainTexture = nullptr;
	}

	RELEASE(samplerLinearClamp)
	RELEASE(samplerLinearWrap)
	RELEASE(terrainTexture)
	RELEASE(grassTexture)
	RELEASE(dustTexture)
}

void RenderableTerrain::create() {

	wstring heightmapPath = wstring(L"res/textures/hqterrain.png");

	D3DX11_IMAGE_INFO hmapInfo;
	HRESULT result;

	if(procedural) {
		int width, height;
		width = height = 150;
		hmapInfo.Width = width;
		hmapInfo.Height = height;

		proceduralTerrainTexture = RenderToTexture(Ctx.device, Ctx.context);
		proceduralTerrainTexture.create(width, height);

		Ctx.setRenderTarget(proceduralTerrainTexture.getRenderTargetView(), NULL);
		Ctx.context->RSSetState(Ctx.RSSolidCullNone);
		Ctx.setViewport(0, 0, proceduralTerrainTexture.getWidth(), proceduralTerrainTexture.getHeight());

		proceduralTerrainTexture.setShaders("VS_QuadObject", "PS_TerrainGeneration_T", "POS3_TEX");
		resources.assignResources(proceduralTerrainTexture.getQuad());
		
		proceduralTerrainTexture.draw();
		Ctx.setRasterizerState(Ctx.currentRasterizerState);

		Ctx.setBackbufferRenderTarget();
		terrainTexture = proceduralTerrainTexture.getShaderResourceView();
	} else {
		D3DX11CreateShaderResourceViewFromFile(Ctx.device, heightmapPath.c_str(), NULL, NULL, &terrainTexture, NULL);
		D3DX11GetImageInfoFromFile(heightmapPath.c_str(), NULL, &hmapInfo, &result);
	}

	//CreateDDSTextureFromFile(Ctx.device, L"res/textures/grass.dds", nullptr, &grassTexture);
	D3DX11CreateShaderResourceViewFromFile(Ctx.device, L"res/textures/greengrasstex.jpg", NULL, NULL, &grassTexture, NULL);
	D3DX11CreateShaderResourceViewFromFile(Ctx.device, L"res/textures/graygrasstex.jpg", NULL, NULL, &dustTexture, NULL);
	DXUtils::createSamplerState(Ctx.device, &samplerLinearClamp, FilterType::D3D11_FILTER_MIN_MAG_MIP_LINEAR, TextureAddressMode::D3D11_TEXTURE_ADDRESS_CLAMP, ComparisonFunction::D3D11_COMPARISON_NEVER);
	DXUtils::createSamplerState(Ctx.device, &samplerLinearWrap, FilterType::D3D11_FILTER_MIN_MAG_MIP_LINEAR, TextureAddressMode::D3D11_TEXTURE_ADDRESS_WRAP, ComparisonFunction::D3D11_COMPARISON_NEVER);

	terrain = Grid(TERRAIN_GRID_SIZE);
	
	vsCB_1.textureSize = XMFLOAT2((float) hmapInfo.Width, (float) hmapInfo.Height);

	terrain.setShaders(TERRAIN_SHADERS);
	terrain.registerObject(Ctx.device, Ctx.context);

	setTweakBars();
}

void RenderableTerrain::render() {
	if(procedural) {
		Ctx.setRenderTarget(proceduralTerrainTexture.getRenderTargetView(), NULL);
		Ctx.context->RSSetState(Ctx.RSSolidCullNone);
		Ctx.setViewport(0, 0, proceduralTerrainTexture.getWidth(), proceduralTerrainTexture.getHeight());
		resources.assignResources(proceduralTerrainTexture.getQuad());

		proceduralTerrainTexture.draw();

		Ctx.setRasterizerState(Ctx.currentRasterizerState);
		Ctx.setBackbufferRenderTarget();
	}

	resources.assignResources(terrain);

	XMMATRIX w = XMMatrixTranspose(terrain.getWorldMatrix());
	XMMATRIX v = XMMatrixTranspose(camera.getViewMatrix());
	XMMATRIX p = XMMatrixTranspose(camera.getProjectionMatrix());

	XMStoreFloat4x4(&vsCB_0.world, w);
	XMStoreFloat4x4(&vsCB_0.view, v);
	XMStoreFloat4x4(&vsCB_0.projection, p);

 	Ctx.setVSResource(terrainTexture, 0);
 	Ctx.setVSSampler(samplerLinearClamp, 0);
 
 	Ctx.setPSResource(grassTexture, 0);
	Ctx.setPSResource(dustTexture, 1);
 	Ctx.setPSSampler(samplerLinearWrap, 0);

	if(procedural) {
		Ctx.setPSResource(proceduralTerrainTexture.getShaderResourceView(), 10);
	}

	shaders.updateConstantBufferVS("CB_Matrices", &vsCB_0, 0);
	shaders.updateConstantBufferVS("CB_TerrainProps", &vsCB_1, 1);

	terrain.draw();

	Ctx.setVSResource(NULL, 0);
	Ctx.setPSResource(NULL, 10);
}

void RenderableTerrain::setProceduralGeneration(bool procedural) {
	this->procedural = procedural;
}

void RenderableTerrain::setTweakBars() {
	terrainBar = TwNewBar("terrainBar");
	TwDefine(" terrainBar visible=false ");
	TwAddVarRW(terrainBar, "Elevation factor", TW_TYPE_FLOAT, &vsCB_1.elevationFactor, "min=0 max=50 step=0.01");
	TwAddVarRW(terrainBar, "Scale factor", TW_TYPE_FLOAT, &vsCB_1.scaleFactor, "min=0 max=50 step=0.01");
	TwAddVarRW(terrainBar, "Texture scale factor", TW_TYPE_FLOAT, &vsCB_1.textureScaleFactor, "min=1 max=50 step=0.01");

	int barPos[2] = { 370, config.CFG_SCREEN_HEIGHT - 130 };

	TwDefine(" terrainBar size='250 90' ");
	TwSetParam(terrainBar, NULL, "position", TW_PARAM_INT32, 2, &barPos);
}
