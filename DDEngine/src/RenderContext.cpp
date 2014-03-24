#include "RenderContext.h"

using namespace DDEngine;

#define RELEASE(X) if(X != NULL) {X->Release();}

RenderContext::RenderContext() {
	setBackgroundColor(255, 255, 255, 255);
	wireframe = false;
	multiSampling = 1;

	depthStencilView = NULL;
	depthStencilBuffer = NULL;
	blendState = NULL;
    renderTargetView = NULL;
    swapChain = NULL;
    context = NULL;
    device = NULL;
	RSSolidCullBack = NULL;
	RSWiredCullNone = NULL;
	RSSolidCullNone = NULL;
}

RenderContext::~RenderContext() {
	if( swapChain ) swapChain->SetFullscreenState(false, NULL);
    if( context ) context->ClearState();

	RELEASE( depthStencilView )
	RELEASE( depthStencilBuffer )
	RELEASE( blendState )
    RELEASE( renderTargetView )
    RELEASE( swapChain )
	RELEASE( RSSolidCullBack )
	RELEASE( RSWiredCullNone )
	RELEASE( RSSolidCullNone )
	RELEASE( context )
	RELEASE( device )
}

void RenderContext::setBlendState( ID3D11BlendState* blendState ) {
	FLOAT blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	context->OMSetBlendState(blendState, blendFactor, 0xffffffff);
}

void RenderContext::setRasterizerState( RasterizerStateType type ) {

	switch (type) {
		case SOLID_CULL_NONE:
			context->RSSetState(RSSolidCullNone);
			break;
		case SOLID_CULL_BACK:
			context->RSSetState(RSSolidCullBack);
			break;
		case WIRED_CULL_NONE:
			context->RSSetState(RSWiredCullNone);
			break;
	}

	currentRasterizerState = type;
}

void RenderContext::setDefaultDepthStencilState() {
	context->OMSetDepthStencilState(0, 0);
}

void RenderContext::setRenderTarget( ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStencilView ) {
	context->OMSetRenderTargets(1, &renderTarget, depthStencilView);
}

void RenderContext::clearRenderTarget( ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStencilView) {
	context->ClearRenderTargetView(renderTarget, bgColor);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderContext::setViewport( int x, int y, int width, int height ) {
	D3D11_VIEWPORT viewPortDesc;

	viewPortDesc.Width = (FLOAT) width;
	viewPortDesc.Height = (FLOAT) height;
	viewPortDesc.MinDepth = 0.0f;
	viewPortDesc.MaxDepth = 1.0f;
	viewPortDesc.TopLeftX = (FLOAT) x;
	viewPortDesc.TopLeftY = (FLOAT) y;

	context->RSSetViewports( 1, &viewPortDesc );
}

void RenderContext::setBackgroundColor( int r, int g, int b, int a ) {
	bgColor[0] = (float) r / 255;
	bgColor[1] = (float) g / 255;
	bgColor[2] = (float) b / 255;
	bgColor[3] = (float) a / 255;
}

void RenderContext::setBackbufferRenderTarget() {
	setRenderTarget(renderTargetView, depthStencilView);
	setViewport(0, 0, screenDimension.WIDTH, screenDimension.HEIGHT);
}

void RenderContext::setRenderWireframe( bool renderedWireframe ) {
	this->wireframe = renderedWireframe;
}

void RenderContext::setPSResource( ID3D11ShaderResourceView* resource, int index ) {
	context->PSSetShaderResources(index, index + 1, &resource);
}

void RenderContext::setVSResource( ID3D11ShaderResourceView* resource, int index ) {
	context->VSSetShaderResources(index, index + 1, &resource);
}

void RenderContext::setPSSampler( ID3D11SamplerState* sampler, int index ) {
	context->PSSetSamplers(index, index + 1, &sampler);
}

void RenderContext::setVSSampler( ID3D11SamplerState* sampler, int index ) {
	context->VSSetSamplers(index, index + 1, &sampler);
}
