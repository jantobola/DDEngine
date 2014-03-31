#include "RenderableTerrain.h"
#include <d3dx11.h>
#include <DirectXTK/DDSTextureLoader.h>

using namespace DDEngine;
using namespace std;
using namespace DirectX;

RenderableTerrain::~RenderableTerrain() {
	terrain.releaseBuffers();
	
	RELEASE(terrainSampler)
	RELEASE(terrainTexture)
	RELEASE(grassTexture)
}

void RenderableTerrain::create() {

	wstring heightmapPath = wstring(L"res/textures/hills.png");

	CreateDDSTextureFromFile(Ctx.device, L"res/textures/grass.dds", nullptr, &grassTexture);
	D3DX11CreateShaderResourceViewFromFile( Ctx.device, heightmapPath.c_str(), NULL, NULL, &terrainTexture, NULL );

	DXUtils::createSamplerState(Ctx.device, &terrainSampler, DXUtils::SamplerType::MIN_MAG_MIP_LINEAR);

	terrain = Grid(TERRAIN_GRID_SIZE);
	
	D3DX11_IMAGE_INFO hmapInfo;
	HRESULT result;
	D3DX11GetImageInfoFromFile(heightmapPath.c_str(), NULL, &hmapInfo, &result);

	vsCB_1.customSize = XMFLOAT3(TERRAIN_SIZE);
	vsCB_1.textureSize = XMFLOAT2((float) hmapInfo.Width, (float) hmapInfo.Height);

	terrain.setShaders(TERRAIN_SHADERS);
	terrain.registerObject(Ctx.device, Ctx.context);
}

void RenderableTerrain::render() {
	camera.update();
	resources.assignResources(terrain);

	XMMATRIX w = XMMatrixTranspose(terrain.getWorldMatrix());
	XMMATRIX v = XMMatrixTranspose(camera.getViewMatrix());
	XMMATRIX p = XMMatrixTranspose(camera.getProjectionMatrix());

	XMStoreFloat4x4(&vsCB_0.world, w);
	XMStoreFloat4x4(&vsCB_0.view, v);
	XMStoreFloat4x4(&vsCB_0.projection, p);

 	Ctx.setVSResource(terrainTexture, 0);
 	Ctx.setVSSampler(terrainSampler, 0);
 
 	Ctx.setPSResource(grassTexture, 0);
 	Ctx.setPSSampler(terrainSampler, 0);

	shaders.updateConstantBufferVS("CB_Matrices", &vsCB_0, 0);
	shaders.updateConstantBufferVS("CB_TerrainProps", &vsCB_1, 1);

	terrain.draw();
}