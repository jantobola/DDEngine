#include "RenderToTexture.h"
#include "DDEUtils.h"
#include "D3DUtils.h"

using namespace DDEngine;

RenderToTexture::RenderToTexture() {

}

RenderToTexture::RenderToTexture(RenderContext& Ctx) {
	this->Ctx = &Ctx;
}

RenderToTexture::~RenderToTexture() {
	cleanUp();
}

HRESULT RenderToTexture::create(int width, int height, DXGI_FORMAT format) {
	this->width = width;
	this->height = height;
	this->format = format;

	HRESULT result  = S_OK;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_QUALITY_LEVELS::D3D11_STANDARD_MULTISAMPLE_PATTERN;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = Ctx->device->CreateTexture2D(&textureDesc, NULL, &texture2D);
	HRESULT_RETURN_CHECK(result)

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = Ctx->device->CreateRenderTargetView(texture2D, &renderTargetViewDesc, &renderTargetView);
	HRESULT_RETURN_CHECK(result);

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = Ctx->device->CreateShaderResourceView(texture2D, &shaderResourceViewDesc, &shaderResourceView);
	
	quad.registerObject("", *Ctx);

	return result;
}

HRESULT DDEngine::RenderToTexture::createDepth() {
	HRESULT result = S_OK;
	result = DXUtils::createDepthStencilBuffer(Ctx->device, Ctx->context, DDEngine::Dimension(width, height), &depthStencilView, &depthStencilBuffer, 1);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = Ctx->device->CreateShaderResourceView(depthStencilBuffer, &shaderResourceViewDesc, &depthShaderResourceView);

	return result;
}

ShaderResourceView* RenderToTexture::getShaderResourceView() {
	return shaderResourceView;
}

ID3D11RenderTargetView* RenderToTexture::getRenderTargetView() {
	return renderTargetView;
}

QuadObject& RenderToTexture::getQuad() {
	return quad;
}

void DDEngine::RenderToTexture::setShaders(std::string vs, std::string ps, std::string il) {
	quad.addShaderCombination("", vs, ps, il);
}

void DDEngine::RenderToTexture::draw() {
	quad.draw();
}

int DDEngine::RenderToTexture::getWidth() {
	return width;
}

int DDEngine::RenderToTexture::getHeight() {
	return height;
}

ID3D11DepthStencilView* DDEngine::RenderToTexture::getDepthStencilView() {
	return depthStencilView;
}

ShaderResourceView* DDEngine::RenderToTexture::getDepthShaderResourceView()
{
	return depthShaderResourceView;
}

void DDEngine::RenderToTexture::setAsRenderTarget()
{
	Ctx->setRenderTarget(renderTargetView, depthStencilView);
	Ctx->setViewport(0, 0, width, height);
}

void DDEngine::RenderToTexture::clearRenderTarget()
{
	float bgcolor[] = { 0, 0, 0, 1 };
	Ctx->clearRenderTarget(bgcolor, renderTargetView, depthStencilView);
}

void RenderToTexture::cleanUp() {
	RELEASE(texture2D)
	RELEASE(shaderResourceView)
	RELEASE(renderTargetView)
	RELEASE(depthStencilBuffer)
	RELEASE(depthStencilView)
	RELEASE(depthShaderResourceView)
}