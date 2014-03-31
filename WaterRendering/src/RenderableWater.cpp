#include "RenderableWater.h"

using namespace DDEngine;
using namespace std;
using namespace DirectX;

RenderableWater::~RenderableWater() {
	waterSurface.releaseBuffers();
	RELEASE(waterSampler)
}

void RenderableWater::create() {

	DXUtils::createSamplerState(Ctx.device, &waterSampler, DXUtils::SamplerType::MIN_MAG_MIP_POINT);

	waterSurface = Grid(size.WIDTH, size.HEIGHT);
	waterSurface.registerObject(Ctx.device, Ctx.context);

	computeTexture_0 = RenderToTexture(Ctx.device, Ctx.context);
	computeTexture_0.create(size.WIDTH, size.HEIGHT);

	computeTexture_1 = RenderToTexture(Ctx.device, Ctx.context);
	computeTexture_1.create(size.WIDTH, size.HEIGHT);

	vsCB_2.sizeX = (float) size.WIDTH;
	vsCB_2.sizeY = (float) size.HEIGHT;

	computeTexture_0.setShaders("VS_QuadObject", "PS_WaterComputation_T", "POS3_TEX");
	computeTexture_1.setShaders("VS_QuadObject", "PS_WaterComputation_T", "POS3_TEX");
	waterSurface.setShaders("VS_WaterVDisplacement", "PS_WaterOptical", "POS2");

	setTweakBars();
}

void RenderableWater::render() {
	XMMATRIX v = XMMatrixTranspose(camera.getViewMatrix());
	XMMATRIX p = XMMatrixTranspose(camera.getProjectionMatrix());

	XMStoreFloat4x4(&vsCB_0.world, waterSurface.getWorldMatrix());
	XMStoreFloat4x4(&vsCB_0.view, v);
	XMStoreFloat4x4(&vsCB_0.projection, p);
	
	vsCB_1.velocity = (float)timer.delta; // some time step

	// COMPUTE SIMULATION (computeTexture_0, RenderToTextureVS, RenderToTexturePS)
	// #################################################################################################################
	
	if (!renderStep || oneStep) {
		vsCB_2.action = 0; // set computation action
		step();
	} else {
		vsCB_2.action = 1;
	}

	Ctx.setRenderTarget(computeTexture_0.getRenderTargetView(), NULL); // where to draw
 	Ctx.setViewport(0, 0, size.WIDTH, size.HEIGHT);

	resources.assignResources(computeTexture_0.getQuad()); // set current shaders
	shaders.updateConstantBufferPS("CB_WaterProps", &vsCB_2, 0); // update CB values
	shaders.updateConstantBufferPS("CB_Timer", &vsCB_1, 1);

	Ctx.setPSResource(computeTexture_1.getShaderResourceView(), 0); // set previous state texture to shader
	Ctx.setPSSampler(waterSampler, 0); // set point sampler

	Ctx.context->RSSetState(Ctx.RSSolidCullNone); // solid no culling

	computeTexture_0.draw(); // compute, store new values
	Ctx.setRasterizerState(Ctx.currentRasterizerState); // set original rasterizer state
 	// #################################################################################################################

 	// RENDER WATER (grid, WaterVS, WaterPS)
	// #################################################################################################################
	Ctx.setBackbufferRenderTarget(); // set original render target
	resources.assignResources(waterSurface); // set current shaders
	
	shaders.updateConstantBufferVS("CB_Matrices", &vsCB_0, 0); // update matrices
	shaders.updateConstantBufferVS("CB_Timer", &vsCB_1, 1); // update time

	ID3D11ShaderResourceView* computedTexture_0 = computeTexture_0.getShaderResourceView(); // set computed texture as resource
	Ctx.setVSResource(computedTexture_0, 0);
	Ctx.setVSSampler(waterSampler, 0);
	Ctx.setPSResource(computedTexture_0, 0);
	Ctx.setPSSampler(waterSampler, 0);

	waterSurface.draw(); // draw grid (vertex displacement)
	// #################################################################################################################

	// COPY (STORE) CURRENT STATE FOR NEXT FRAME (computeTexture_1, RenderToTextureVS, RenderToTexturePS)
	// #################################################################################################################
	vsCB_2.action = 1; // set copy action
	Ctx.setRenderTarget(computeTexture_1.getRenderTargetView(), NULL); // where to draw
	Ctx.setViewport(0, 0, size.WIDTH, size.HEIGHT);

	resources.assignResources(computeTexture_1.getQuad()); // set current shaders
	shaders.updateConstantBufferPS("CB_WaterProps", &vsCB_2, 0); // update CB values

	Ctx.setPSResource(computeTexture_0.getShaderResourceView(), 0); // set previous state texture to shader
	Ctx.setPSSampler(waterSampler, 0); // set point sampler

	Ctx.context->RSSetState(Ctx.RSSolidCullNone); // solid no culling
	computeTexture_1.draw(); // compute, store new values
	Ctx.setRasterizerState(Ctx.currentRasterizerState); // set original rasterizer state
	// #################################################################################################################

	// unmap resource views
 	computedTexture_0 = NULL;
 	Ctx.setVSResource(computedTexture_0, 0);
	Ctx.setPSResource(computedTexture_0, 0);

	Ctx.setBackbufferRenderTarget();
	setWaterDrop(false); // turn off water drop
	resetSurface(false); // switch reset state to ignore in next frames
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

	TwDefine(" waterBar size='200 50' ");
	TwSetParam(waterBar, NULL, "position", TW_PARAM_INT32, 2, &barPos);
	TwDefine(" waterBar visible=false ");
	TwAddVarRW(waterBar, "pointHeight", TW_TYPE_FLOAT, &vsCB_2.height, "min=0 max=10 step=0.001");
	TwAddVarRW(waterBar, "viscosity", TW_TYPE_FLOAT, &vsCB_2.viscosity, "min=0 max=1 step=0.0001");
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