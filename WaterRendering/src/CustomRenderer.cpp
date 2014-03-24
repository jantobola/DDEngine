#include "CustomRenderer.h"
#include "RenderableWater.h"
#include "RenderableTerrain.h"
#include "RenderableSkybox.h"

using namespace DDEngine;
using namespace std;
using namespace DirectX;

CustomRenderer::CustomRenderer(std::string configPath) : DDEComponent(configPath) {
	terrain = NULL;
	skybox = NULL;
	water = NULL;
}

CustomRenderer::~CustomRenderer() {
	cleanUp();
}

void CustomRenderer::create() {
	initShaders();
	DDERenderPackage pkg = getRenderPackage();

	skybox = new RenderableSkybox(pkg);
	terrain = new RenderableTerrain(pkg);
	water = new RenderableWater(pkg);

	skybox->setName("skybox");
	terrain->setName("terrain");
	water->setName("water");

	skybox->setSkyboxPath("res/textures/Skybox.dds");	
	water->setSize(400, 400);

	objects->addObject(skybox);
	objects->addObject(terrain);
	objects->addObject(water);

	// Tweak bar and light init values

	tweakBar = TwNewBar("lightBar");

	int barPos[2] = { 10, config.CFG_SCREEN_HEIGHT - 200 };

	TwDefine(" lightBar size='350 160' ");
	TwSetParam(tweakBar, NULL, "position", TW_PARAM_INT32, 2, &barPos);

	light.direction = XMFLOAT3(0.0f, -1.0f, -1.0f);
	light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = XMFLOAT4(1, 1, 1, 1.0f);
	XMStoreFloat4x4(&light.identity, XMMatrixIdentity());

	TwAddVarRW(tweakBar, "lightDirection", TW_TYPE_DIR3F, &light.direction, "opened=true");
	TwDefine(" lightBar visible=false valueswidth=150");
}

void CustomRenderer::render() {
 	shaders->updateConstantBufferPS("perFrame", &light, 5);
}

void CustomRenderer::initShaders() {
	wstring	path = L"shaders/";

	#ifdef DEBUG
		path = L"../../../src/shaders/";
	#endif
	#ifdef PRECOMPILED_SHADERS
		path = L"shaders/";
	#endif

	#define _shader(X) path + L#X

	#ifdef PRECOMPILED_SHADERS
		shaders->addVertexShaderBinary("MeshVS", _shader(MeshVS.cso));
		shaders->addPixelShaderBinary("MeshPS", _shader(MeshPS.cso));

		shaders->addVertexShaderBinary("TerrainVS", _shader(TerrainVS.cso));

		shaders->addVertexShaderBinary("WaterVS", _shader(WaterVS.cso));
		shaders->addPixelShaderBinary("WaterPS", _shader(WaterPS.cso));

		shaders->addVertexShaderBinary("RenderToTextureVS", _shader(RenderToTextureVS.cso));
		shaders->addPixelShaderBinary("RenderToTexturePS", _shader(RenderToTexturePS.cso));

		shaders->addVertexShaderBinary("SkyboxVS", _shader(SkyboxVS.cso));
		shaders->addPixelShaderBinary("SkyboxPS", _shader(SkyboxPS.cso));
	#else
		shaders->addVertexShader("MeshVS", _shader(MeshVS.hlsl));
		shaders->addPixelShader("MeshPS", _shader(MeshPS.hlsl));

		shaders->addVertexShader("TerrainVS", _shader(TerrainVS.hlsl));

		shaders->addVertexShader("WaterVS", _shader(WaterVS.hlsl));
		shaders->addPixelShader("WaterPS", _shader(WaterPS.hlsl));

		shaders->addVertexShader("RenderToTextureVS", _shader(RenderToTextureVS.hlsl));
		shaders->addPixelShader("RenderToTexturePS", _shader(RenderToTexturePS.hlsl));

		shaders->addVertexShader("SkyboxVS", _shader(SkyboxVS.hlsl));
		shaders->addPixelShader("SkyboxPS", _shader(SkyboxPS.hlsl));
	#endif

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	D3D11_INPUT_ELEMENT_DESC layout2[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC layout3[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	#ifdef PRECOMPILED_SHADERS
		shaders->addInputLayoutBinary("POS3_TEX_NOR", "MeshVS", layout, ARRAYSIZE(layout));
		shaders->addInputLayoutBinary("POS4_TEX", "WaterVS", layout2, ARRAYSIZE(layout2));
		shaders->addInputLayoutBinary("POS3_TEX", "SkyboxVS", layout, ARRAYSIZE(layout));
	#else
		shaders->addInputLayout("POS3_TEX_NOR", "MeshVS", layout, ARRAYSIZE(layout));
		shaders->addInputLayout("POS4_TEX", "WaterVS", layout2, ARRAYSIZE(layout2));
		shaders->addInputLayout("POS3_TEX", "SkyboxVS", layout, ARRAYSIZE(layout));
	#endif

	shaders->addConstantBuffer("perFrame", sizeof(Light_CB));
	shaders->addConstantBuffer("WVP", sizeof(WVP_CB));
	shaders->addConstantBuffer("heightMap", sizeof(Terrain_CB));
	shaders->addConstantBuffer("timer", sizeof(Timer_CB));
	shaders->addConstantBuffer("perObject", sizeof(Matrices_CB));
	shaders->addConstantBuffer("waterProps", sizeof(WaterProps_CB));
}

void CustomRenderer::cleanUp() {
	delete terrain;
	delete skybox;
	delete water;
}