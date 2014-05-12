#include "RenderToTexture.h"
#include "DDEUtils.h"

using namespace DDEngine;

RenderToTexture::RenderToTexture() {

}

RenderToTexture::RenderToTexture(ID3D11Device* device, ID3D11DeviceContext* context) {
	this->device = device;
	this->context = context;
}

RenderToTexture::~RenderToTexture() {
	quad.releaseBuffers();
	cleanUp();
}

HRESULT RenderToTexture::create( int width, int height ) {
	this->width = width;
	this->height = height;

	HRESULT result  = S_OK;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&textureDesc, NULL, &texture2D);
	HRESULT_RETURN_CHECK(result)

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateRenderTargetView(texture2D, &renderTargetViewDesc, &renderTargetView);
	HRESULT_RETURN_CHECK(result);

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(texture2D, &shaderResourceViewDesc, &shaderResourceView);
	
	quad.registerObject(device, context);

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
	quad.setShaders(vs, ps, il);
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

void RenderToTexture::cleanUp() {
	RELEASE(texture2D)
	RELEASE(shaderResourceView)
	RELEASE(renderTargetView)
}