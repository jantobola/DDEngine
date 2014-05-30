#include "CustomRenderer.h"
#include "RenderableWater.h"
#include "RenderableTerrain.h"
#include <RenderableSkybox.h>
#include <DDEngine.h>

using namespace DDEngine;
using namespace std;
using namespace DirectX;

CustomRenderer::CustomRenderer(std::string configPath) : DDEComponent(configPath) {
	
}

CustomRenderer::~CustomRenderer() {
	cleanUp();
}

void CustomRenderer::create() {
	initShaders();
	DDERenderPackage pkg = getRenderPackage();

	skybox = new DDEngine::RenderableSkybox(pkg);
	terrain = new RenderableTerrain(pkg);
	water = new RenderableWater(pkg);
	
	water->setSkybox(skybox);
	water->setTerrain(terrain);

	skybox->setName("skybox");
	terrain->setName("terrain");
	water->setName("water");

	skybox->setSkyboxPath("res/textures/TropicalDaySkybox.dds");
	terrain->setProceduralGeneration(false);

	objects->addObject(skybox);
	objects->addObject(terrain);
	objects->addObject(water);

	setTweakBars();
}

void CustomRenderer::render() {
 	shaders->updateConstantBufferPS("CB_LightProps", &light, 10);
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
		shaders->addVertexShaderBinary("VS_BasicMesh", _shader(VS_BasicMesh.cso));
		shaders->addPixelShaderBinary("PS_BasicLightMesh", _shader(PS_BasicLightMesh.cso));

		shaders->addVertexShaderBinary("VS_TerrainVDisplacement", _shader(VS_TerrainVDisplacement.cso));
		shaders->addPixelShaderBinary("PS_TerrainGeneration_T", _shader(PS_TerrainGeneration_T.cso));

		shaders->addVertexShaderBinary("VS_WaterVDisplacement", _shader(VS_WaterVDisplacement.cso));
		shaders->addPixelShaderBinary("PS_WaterOptical", _shader(PS_WaterOptical.cso));

		shaders->addVertexShaderBinary("VS_QuadObject", _shader(VS_QuadObject.cso));
		shaders->addPixelShaderBinary("PS_WaterComputation_T", _shader(PS_WaterComputation_T.cso));

	#else
		shaders->addVertexShader("VS_BasicMesh", _shader(VS_BasicMesh.hlsl));
		shaders->addPixelShader("PS_BasicLightMesh", _shader(PS_BasicLightMesh.hlsl));

		shaders->addVertexShader("VS_TerrainVDisplacement", _shader(VS_TerrainVDisplacement.hlsl));
		shaders->addPixelShader("PS_TerrainGeneration_T", _shader(PS_TerrainGeneration_T.hlsl));

		shaders->addVertexShader("VS_WaterVDisplacement", _shader(VS_WaterVDisplacement.hlsl));
		shaders->addVertexShader("VS_WaterBottom", _shader(VS_WaterBottom.hlsl));
		shaders->addPixelShader("PS_WaterOptical", _shader(PS_WaterOptical.hlsl));

		shaders->addVertexShader("VS_QuadObject", _shader(VS_QuadObject.hlsl));
		shaders->addPixelShader("PS_WaterComputation_T", _shader(PS_WaterComputation_T.hlsl));

	#endif

	D3D11_INPUT_ELEMENT_DESC layout1[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	D3D11_INPUT_ELEMENT_DESC layout3[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	#ifdef PRECOMPILED_SHADERS
		shaders->addInputLayoutBinary("POS3_TEX_NOR", "VS_BasicMesh", layout1, ARRAYSIZE(layout1));
		shaders->addInputLayoutBinary("POS2", "VS_TerrainVDisplacement", layout3, ARRAYSIZE(layout3));
	#else
		shaders->addInputLayout("POS3_TEX_NOR", "VS_BasicMesh", layout1, ARRAYSIZE(layout1));
		shaders->addInputLayout("POS2", "VS_TerrainVDisplacement", layout3, ARRAYSIZE(layout3));
	#endif

	shaders->addConstantBuffer("CB_LightProps", sizeof(Light_CB));
	shaders->addConstantBuffer("CB_TerrainProps", sizeof(Terrain_CB));
	shaders->addConstantBuffer("CB_Timer", sizeof(Timer_CB));
	shaders->addConstantBuffer("CB_Matrices", sizeof(Matrices_CB));
	shaders->addConstantBuffer("CB_WaterProps", sizeof(WaterProps_CB));
	shaders->addConstantBuffer("CB_Camera", sizeof(Camera_CB));
}

void CustomRenderer::cleanUp() {
	delete terrain;
	delete skybox;
	delete water;
}

void CustomRenderer::setTweakBars() {
	tweakBar = TwNewBar("lightBar");

	int barPos[2] = { 10, config.CFG_SCREEN_HEIGHT - 200 };

	TwDefine(" lightBar size='350 160' ");
	TwSetParam(tweakBar, nullptr, "position", TW_PARAM_INT32, 2, &barPos);

	light.direction = XMFLOAT3(0.0f, -1.0f, -1.0f);
	light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = XMFLOAT4(1, 1, 1, 1.0f);

	TwAddVarRW(tweakBar, "lightDirection", TW_TYPE_DIR3F, &light.direction, "opened=true");
	TwDefine(" lightBar visible=false valueswidth=150");
}
