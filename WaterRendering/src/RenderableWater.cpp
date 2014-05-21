#include "RenderableWater.h"
#include <d3dx11.h>
#include <DirectXMath.h>
#include "RenderableSkybox.h"
#include "RenderableTerrain.h"
#include <HUDRenderer.h>

using namespace DDEngine;
using namespace std;
using namespace DirectX;

RenderableWater::~RenderableWater() {
	waterSurface.releaseBuffers();

	RELEASE(samplerPointClamp)
	RELEASE(samplerLinearClamp)
	RELEASE(samplerLinearWrap)
}

void RenderableWater::create() {

	hud.loadingScreen("Loading water resources...");

	setSize(1024, 1024);

	DXUtils::createSamplerState(Ctx.device, &samplerPointClamp, FilterType::D3D11_FILTER_MIN_MAG_MIP_POINT, TextureAddressMode::D3D11_TEXTURE_ADDRESS_CLAMP, ComparisonFunction::D3D11_COMPARISON_NEVER);
	DXUtils::createSamplerState(Ctx.device, &samplerLinearClamp, FilterType::D3D11_FILTER_MIN_MAG_MIP_LINEAR, TextureAddressMode::D3D11_TEXTURE_ADDRESS_CLAMP, ComparisonFunction::D3D11_COMPARISON_NEVER);
	DXUtils::createSamplerState(Ctx.device, &samplerLinearWrap, FilterType::D3D11_FILTER_MIN_MAG_MIP_LINEAR, TextureAddressMode::D3D11_TEXTURE_ADDRESS_WRAP, ComparisonFunction::D3D11_COMPARISON_NEVER);

	waterSurface = Grid(size.WIDTH, size.HEIGHT);
	waterSurface.registerObject(Ctx.device, Ctx.context);

	computeTexture_0 = RenderToTexture(Ctx.device, Ctx.context);
	computeTexture_0.create(size.WIDTH, size.HEIGHT);

	computeTexture_1 = RenderToTexture(Ctx.device, Ctx.context);
	computeTexture_1.create(size.WIDTH, size.HEIGHT);

	computeTexture_2 = RenderToTexture(Ctx.device, Ctx.context);
	computeTexture_2.create(size.WIDTH, size.HEIGHT);

	computeTexture_3 = RenderToTexture(Ctx.device, Ctx.context);
	computeTexture_3.create(size.WIDTH, size.HEIGHT);

	vsCB_2.sizeX = (float) size.WIDTH;
	vsCB_2.sizeY = (float) size.HEIGHT;

	computeTexture_0.setShaders("VS_QuadObject", "PS_WaterComputation_T", "POS3_TEX");
	computeTexture_1.setShaders("VS_QuadObject", "PS_WaterComputation_T", "POS3_TEX");
	computeTexture_2.setShaders("VS_QuadObject", "PS_WaterComputation_T", "POS3_TEX");
	computeTexture_3.setShaders("VS_QuadObject", "PS_WaterComputation_T", "POS3_TEX");
	waterSurface.setShaders("VS_WaterVDisplacement", "PS_WaterOptical", "POS2");

	setTweakBars();
}

void RenderableWater::render() {

	XMMATRIX v = XMMatrixTranspose(camera.getViewMatrix());
	XMMATRIX p = XMMatrixTranspose(camera.getProjectionMatrix());

	XMStoreFloat4x4(&vsCB_0.view, v);
	XMStoreFloat4x4(&vsCB_0.projection, p);

	vsCB_1.time = (double) timer.delta;
	vsCB_1.timeCycle += (double) timer.delta;

	// ############################ ACTION 0 ############################

	if (isStep()) {

		setAction(0);

		Ctx.setRenderTarget(computeTexture_0.getRenderTargetView(), nullptr); // where to draw
		Ctx.setViewport(0, 0, size.WIDTH, size.HEIGHT);

		resources.assignResources(computeTexture_0.getQuad()); // set current shaders
		shaders.updateConstantBufferPS("CB_WaterProps", &vsCB_2, 0); // update CB values
		shaders.updateConstantBufferPS("CB_Timer", &vsCB_1, 1);
		shaders.updateConstantBufferPS("CB_TerrainProps", &terrain->vsCB_1, 2);

		Ctx.setPSResource(computeTexture_2.getShaderResourceView(), 0); // set previous state texture to shader
		Ctx.setPSResource(computeTexture_3.getShaderResourceView(), 1);
		Ctx.setPSResource(terrain->terrainTexture, 2);
		Ctx.setPSSampler(samplerPointClamp, 0); // set point sampler

		Ctx.context->RSSetState(Ctx.RSSolidCullNone); // solid no culling
		Ctx.setBlendState(nullptr); // disable blending to store correct values in all color channels

		computeTexture_0.draw(); // compute, store new values

		// ############################ ACTION 1 #############################

		setAction(1);

		shaders.updateConstantBufferPS("CB_WaterProps", &vsCB_2, 0);

		Ctx.setRenderTarget(computeTexture_1.getRenderTargetView(), nullptr);
		Ctx.setPSResource(computeTexture_0.getShaderResourceView(), 0);
		Ctx.setPSResource(computeTexture_3.getShaderResourceView(), 1);

		computeTexture_1.draw();

		Ctx.setBlendState(Ctx.blendState);
		Ctx.setRasterizerState(Ctx.currentRasterizerState); // set original rasterizer state
	}

 	// ######################################################################
 	// RENDER WATER (grid, WaterVS, WaterPS)
	// ######################################################################

	Ctx.setBackbufferRenderTarget(); // set original render target
	resources.assignResources(waterSurface); // set current shaders

	XMStoreFloat4x4(&vsCB_0.world, waterSurface.getWorldMatrix());
	//XMStoreFloat4x4(&vsCB_0.worldInvTrans, XMMatrixTranspose(XMMatrixInverse(NULL, waterSurface.getWorldMatrix())));

	vsCB_3.cameraPosition = camera.eye;

	shaders.updateConstantBufferVS("CB_Matrices", &vsCB_0, 0); // update matrices
	shaders.updateConstantBufferVS("CB_Timer", &vsCB_1, 1); // update time
	shaders.updateConstantBufferVS("CB_WaterProps", &vsCB_2, 2); // update surface props
	shaders.updateConstantBufferVS("CB_Camera", &vsCB_3, 3); // update camera vectors
	shaders.updateConstantBufferVS("CB_TerrainProps", &terrain->vsCB_1, 4);

	// set computed texture as resource
	Ctx.setVSResource(computeTexture_1.getShaderResourceView(), 0);
	Ctx.setVSResource(terrain->terrainTexture, 1);
	Ctx.setVSSampler(samplerPointClamp, 0);
	Ctx.setPSResource(skybox->getSkyboxTexture(), 0);
	Ctx.setPSResource(terrain->perspectiveView.getShaderResourceView(), 1);
	Ctx.setPSResource(terrain->terrainTexture, 2);
	Ctx.setPSSampler(samplerLinearWrap, 0);
	Ctx.setPSSampler(samplerLinearClamp, 1);

	waterSurface.draw(); // draw grid (vertex displacement)

	Ctx.setVSResource(nullptr, 0);
	Ctx.setVSResource(nullptr, 1);
	Ctx.setPSResource(nullptr, 0);
	Ctx.setPSResource(nullptr, 1);
	Ctx.setPSResource(nullptr, 2);
	Ctx.setPSSampler(nullptr, 0);

	// ############################# ACTION 2 #############################
	
	// COPY 1

	setAction(2); // set copy action
	Ctx.setRenderTarget(computeTexture_2.getRenderTargetView(), nullptr); // where to draw
	Ctx.setViewport(0, 0, size.WIDTH, size.HEIGHT);
	Ctx.setBlendState(nullptr);

	resources.assignResources(computeTexture_2.getQuad()); // set current shaders
	shaders.updateConstantBufferPS("CB_WaterProps", &vsCB_2, 0); // update CB values

	Ctx.setPSResource(computeTexture_0.getShaderResourceView(), 3); // set previous state texture to shader
	Ctx.setPSSampler(samplerPointClamp, 0); // set point sampler

	Ctx.context->RSSetState(Ctx.RSSolidCullNone); // solid no culling

	computeTexture_2.draw(); // compute, store new values
	
	// COPY 2

	Ctx.setRenderTarget(computeTexture_3.getRenderTargetView(), nullptr);
	Ctx.setPSResource(computeTexture_1.getShaderResourceView(), 3);
	
	computeTexture_3.draw();
	
	
	Ctx.setRasterizerState(Ctx.currentRasterizerState); // set original rasterizer state
	Ctx.setBlendState(Ctx.blendState);
	
	// #####################################################################

	// unmap resource views
 	Ctx.setVSResource(nullptr, 0);
	Ctx.setVSResource(nullptr, 1);
	Ctx.setPSResource(nullptr, 0);
	Ctx.setPSResource(nullptr, 1);
	Ctx.setPSResource(nullptr, 2);
	Ctx.setPSResource(nullptr, 3);

	Ctx.setBackbufferRenderTarget();
	setWaterDrop(false); // turn off water drop
	resetSurface(false); // switch reset state to ignore in next frames

	if (vsCB_1.timeCycle > vsCB_1.timeStep) {
		vsCB_1.timeCycle = 0.0;
	}

}

void RenderableWater::setSize( int width, int height ) {
	size.WIDTH = (UINT) width;
	size.HEIGHT = (UINT) height;
}

Dimension RenderableWater::getSize() {
	return this->size;
}

void RenderableWater::setTweakBars()
{
	waterBar = TwNewBar("waterBar");

	int barPos[2] = { config.CFG_SCREEN_WIDTH - 210, 10 };

	TwDefine(" waterBar size='200 120' ");
	TwSetParam(waterBar, nullptr, "position", TW_PARAM_INT32, 2, &barPos);
	TwDefine(" waterBar visible=false ");
	TwAddVarRW(waterBar, "Drop Strength", TW_TYPE_FLOAT, &vsCB_2.height, "min=0 max=1 step=0.0001");
	TwAddVarRW(waterBar, "Viscosity", TW_TYPE_FLOAT, &vsCB_2.viscosity, "min=0 max=1 step=0.0001");
	TwAddVarRW(waterBar, "Time Step", TW_TYPE_FLOAT, &vsCB_1.timeStep, "min=0 max=20000 step=1");
}

void RenderableWater::setWaterDrop(bool isOn) {
	if(isOn) vsCB_2.waterDrop = 1;
	else vsCB_2.waterDrop = 0;
}

void RenderableWater::resetSurface(bool reset) {
	if (reset) vsCB_2.reset = 1;
	else vsCB_2.reset = 0;
}

void RenderableWater::setStepping(bool stepping) {
	renderStep = stepping;
}

void RenderableWater::step() {
	if (oneStep) {
		oneStep = false;
	}
	else {
		oneStep = true;
	}
}

bool RenderableWater::isStep() {
	if (!renderStep || oneStep) {
		step();
		return true;
	}
	else return false;
}

void RenderableWater::setAction(int action) {
	vsCB_2.action = action;
}