#include "Skybox.h"
#include "D3DUtils.h"
#include "DDEUtils.h"
#include "Camera.h"

using namespace DDEngine;
using namespace std;
using namespace DirectX;

Skybox::~Skybox() {
	cleanUp();
}

void Skybox::initContext() {

	DXUtils::createCubeTextureResource(Ctx->device, StringUtils::toWstring(ddsPath).c_str(), &skyboxTexture);
	DXUtils::createSamplerState(Ctx->device, &skyboxSampler, FilterType::D3D11_FILTER_MIN_MAG_MIP_LINEAR, TextureAddressMode::D3D11_TEXTURE_ADDRESS_CLAMP, ComparisonFunction::D3D11_COMPARISON_NEVER);
	DXUtils::createRasterizerState(Ctx->device, &skyboxRasterizer, D3D11_CULL_NONE, D3D11_FILL_SOLID, 0);
	
	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
	ZeroMemory(&depthStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStateDesc.DepthEnable = true;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	Ctx->device->CreateDepthStencilState(&depthStateDesc, &skyboxDepth);
}

void Skybox::setDDSTexturePath( std::string texturePath ) {
	this->ddsPath = texturePath;
}

void Skybox::loadGeometry(std::vector<Mesh>& meshes) {

	initContext();
	Mesh mesh;

#define VB(x, y, z, u, v) mesh.VB(VertexPositionTexture(XMFLOAT3(x, y, z), XMFLOAT2(u, v)))
#define IB(i) mesh.IB(i)

	//XMStoreFloat4x4(&scaleMatrix, XMMatrixScaling(20, 20, 20));
	//XMStoreFloat4x4(&rotationMatrix, XMMatrixRotationY(XMConvertToRadians(240)));

	// ---------- Vertex Buffer ------------

	// FRONT FACE
	VB( /* POS */ -1, -1, -1, /* TEX */ 0, 1);
	VB( /* POS */ -1, +1, -1, /* TEX */ 0, 0);
	VB( /* POS */ +1, +1, -1, /* TEX */ 1, 0);
	VB( /* POS */ +1, -1, -1, /* TEX */ 1, 1);

	// BACK FACE
	VB( /* POS */ -1, -1, 1, /* TEX */ 1, 1);
	VB( /* POS */  1, -1, 1, /* TEX */ 0, 1);
	VB( /* POS */  1, 1, 1, /* TEX */ 0, 0);
	VB( /* POS */ -1, 1, 1, /* TEX */ 1, 0);

	// TOP FACE
	VB( /* POS */ -1, 1, -1, /* TEX */ 0, 1);
	VB( /* POS */ -1, 1, 1, /* TEX */ 0, 0);
	VB( /* POS */  1, 1, 1, /* TEX */ 1, 0);
	VB( /* POS */  1, 1, -1, /* TEX */ 1, 1);

	// BOTTOM FACE
	VB( /* POS */ -1, -1, -1, /* TEX */ 1, 1);
	VB( /* POS */  1, -1, -1, /* TEX */ 0, 1);
	VB( /* POS */  1, -1, 1, /* TEX */ 0, 0);
	VB( /* POS */ -1, -1, 1, /* TEX */ 1, 0);

	// LEFT FACE
	VB( /* POS */ -1, -1, 1, /* TEX */ 0, 1);
	VB( /* POS */ -1, 1, 1, /* TEX */ 0, 0);
	VB( /* POS */ -1, 1, -1, /* TEX */ 1, 0);
	VB( /* POS */ -1, -1, -1, /* TEX */ 1, 1);

	// RIGHT FACE
	VB( /* POS */  1, -1, -1, /* TEX */ 0, 1);
	VB( /* POS */  1, 1, -1, /* TEX */ 0, 0);
	VB( /* POS */  1, 1, 1, /* TEX */ 1, 0);
	VB( /* POS */  1, -1, 1, /* TEX */ 1, 1);

	// ------------ Index Buffer ---------------

	// FRONT FACE
	IB(0);  IB(1);  IB(2);
	IB(0);  IB(2);  IB(3);

	// BACK FACE
	IB(4);  IB(5);  IB(6);
	IB(4);  IB(6);  IB(7);

	// TOP FACE
	IB(8);  IB(9);  IB(10);
	IB(8);  IB(10); IB(11);

	// BOTTOM FACE
	IB(12); IB(13); IB(14);
	IB(12); IB(14); IB(15);

	// LEFT FACE
	IB(16); IB(17); IB(18);
	IB(16); IB(18); IB(19);

	// RIGHT FACE
	IB(20); IB(21); IB(22);
	IB(20); IB(22); IB(23);

	meshes.push_back(mesh);
}

void Skybox::draw() {
	Ctx->context->PSSetShaderResources(0, 1, &skyboxTexture);
	Ctx->context->PSSetSamplers(0, 1, &skyboxSampler);
	Ctx->context->OMSetDepthStencilState(skyboxDepth, 0);
	Ctx->context->RSSetState(skyboxRasterizer);

	Object3D::draw();
}

DirectX::XMMATRIX Skybox::getSkyboxWVP( Camera& camera ) {
	XMStoreFloat4x4(&translationMatrix, XMMatrixTranslation(camera.eye.x, camera.eye.y, camera.eye.z));
	XMMATRIX wvp = Object3D::getWorldMatrix() * camera.getViewMatrix() * camera.getProjectionMatrix();
	return XMMatrixTranspose(wvp);
}

void Skybox::cleanUp() {
	RELEASE(skyboxTexture)
	RELEASE(skyboxSampler)
	RELEASE(skyboxDepth)
	RELEASE(skyboxRasterizer)
}

ID3D11ShaderResourceView* DDEngine::Skybox::getSkyboxTexture()
{
	return skyboxTexture;
}
