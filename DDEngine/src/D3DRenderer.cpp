#include "D3DRenderer.h"
#include "D3DUtils.h"
#include "DDEUtils.h"

using namespace DDEngine;

D3DRenderer::D3DRenderer() {
    
}

D3DRenderer::~D3DRenderer() {
	cleanUp();
}

HRESULT D3DRenderer::initialize(HWND hwnd) {
	
    HRESULT result = S_OK;
	Ctx.screenDimension = screenDimension = DDEUtils::getDimensionFromHandle(hwnd);

    result = DXUtils::createDeviceAndSwapChain(&Ctx.device, &Ctx.context, &Ctx.swapChain, &Ctx.renderTargetView, hwnd, Ctx.multiSampling);
	Win32Utils::showFailMessage(result, "Initialization error", "Cannot create Direct3D device.");
	if (FAILED(result)) PostQuitMessage(-1);

	result = DXUtils::createDepthStencilBuffer(Ctx.device, Ctx.context, screenDimension, &Ctx.depthStencilView, &Ctx.depthStencilBuffer, Ctx.multiSampling);
	Win32Utils::showFailMessage(result, "Initialization error", "Cannot create Direct3D depth stencil buffer.");

	Ctx.setRenderTarget(Ctx.renderTargetView, Ctx.depthStencilView);
	Ctx.setViewport(0, 0, screenDimension.WIDTH, screenDimension.HEIGHT);

	result = DXUtils::createRasterizerState(Ctx.device, &Ctx.RSSolidCullBack, D3D11_CULL_BACK, D3D11_FILL_SOLID, 0);
	Win32Utils::showFailMessage(result, "Initialization error", "Cannot create Direct3D rasterizer state.");
	result = DXUtils::createRasterizerState(Ctx.device, &Ctx.RSWiredCullNone, D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, 0);
	Win32Utils::showFailMessage(result, "Initialization error", "Cannot create Direct3D rasterizer state.");
	result = DXUtils::createRasterizerState(Ctx.device, &Ctx.RSSolidCullNone, D3D11_CULL_NONE, D3D11_FILL_SOLID, 0);
	Win32Utils::showFailMessage(result, "Initialization error", "Cannot create Direct3D rasterizer state.");

	Ctx.setRasterizerState(RasterizerStateType::SOLID_CULL_BACK);

	result = DXUtils::createBlendState(Ctx.device, &Ctx.blendState);
	Win32Utils::showFailMessage(result, "Initialization error", "Cannot create Direct3D blend state.");

	Ctx.commonStates = new DirectX::CommonStates(Ctx.device);

	camera.setProjectionMatrices(screenDimension);
    return result;
}

void D3DRenderer::renderFrame() {
	
	Ctx.setDefaultDepthStencilState();
	Ctx.setBlendState(Ctx.blendState);

	Ctx.clearRenderTarget(Ctx.renderTargetView, Ctx.depthStencilView);
	
	if(!Ctx.wireframe) {
		Ctx.setRasterizerState(RasterizerStateType::SOLID_CULL_BACK);
	} else {
		Ctx.setRasterizerState(RasterizerStateType::WIRED_CULL_NONE);
	}
	
	onRender();
	Ctx.swapChain->Present( 0, 0 );
}

void D3DRenderer::cleanUp() {
	delete Ctx.commonStates;
}

Camera& D3DRenderer::getCamera() {
	return camera;
}

Timer& D3DRenderer::getTimer() {
	return timer;
}

void D3DRenderer::setRenderWireframe(bool renderedWireframe) {
	Ctx.setRenderWireframe(renderedWireframe);
}

void D3DRenderer::setBackgroundColor( int r, int g, int b, int a ) {
	Ctx.setBackgroundColor(r, g, b, a);
}

void DDEngine::D3DRenderer::setMultiSamplingValue(UINT multiSampling) {
	Ctx.multiSampling = multiSampling;
}