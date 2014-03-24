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

	DXUtils::createCubeTextureResource(device, StringUtils::toWstring(ddsPath).c_str(), &skyboxTexture);
	DXUtils::createSamplerState(device, &skyboxSampler, DXUtils::SamplerType::MIN_MAG_MIP_LINEAR);
	DXUtils::createRasterizerState(device, &skyboxRasterizer, D3D11_CULL_NONE, D3D11_FILL_SOLID, 0);
	
	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
	ZeroMemory(&depthStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStateDesc.DepthEnable = true;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&depthStateDesc, &skyboxDepth);
}

void Skybox::setDDSTexturePath( std::string texturePath ) {
	this->ddsPath = texturePath;
}

void Skybox::loadGeometry(vector<Vertex>* vertices, vector<DWORD>* indices) {
#define VB(x, y, z, u, v, nx, ny, nz) vertices->push_back(Vertex(x, y, z, u, v, nx, ny, nz));
#define IB(I) indices->push_back(I);

	initContext();

	//XMStoreFloat4x4(&scaleMatrix, XMMatrixScaling(20, 20, 20));
	//XMStoreFloat4x4(&rotationMatrix, XMMatrixRotationY(XMConvertToRadians(240)));

	// ---------- Vertex Buffer ------------

	// FRONT FACE
	VB( /* POS */ -1, -1, -1, /* TEX */ 0, 1, /* NOR */ -1, -1, -1 )
	VB( /* POS */ -1,  1, -1, /* TEX */ 0, 0, /* NOR */ -1,  1, -1 )
	VB( /* POS */  1,  1, -1, /* TEX */ 1, 0, /* NOR */  1,  1, -1 )
	VB( /* POS */  1, -1, -1, /* TEX */ 1, 1, /* NOR */  1, -1, -1 )

	// BACK FACE
	VB( /* POS */ -1, -1,  1, /* TEX */ 1, 1, /* NOR */ -1, -1,  1 )
	VB( /* POS */  1, -1,  1, /* TEX */ 0, 1, /* NOR */  1, -1,  1 )
	VB( /* POS */  1,  1,  1, /* TEX */ 0, 0, /* NOR */  1,  1,  1 )
	VB( /* POS */ -1,  1,  1, /* TEX */ 1, 0, /* NOR */ -1,  1,  1 )

	// TOP FACE
	VB( /* POS */ -1,  1, -1, /* TEX */ 0, 1, /* NOR */ -1,  1, -1 )
	VB( /* POS */ -1,  1,  1, /* TEX */ 0, 0, /* NOR */ -1,  1,  1 )
	VB( /* POS */  1,  1,  1, /* TEX */ 1, 0, /* NOR */  1,  1,  1 )
	VB( /* POS */  1,  1, -1, /* TEX */ 1, 1, /* NOR */  1,  1, -1 )

	// BOTTOM FACE
	VB( /* POS */ -1, -1, -1, /* TEX */ 1, 1, /* NOR */ -1, -1, -1 )
	VB( /* POS */  1, -1, -1, /* TEX */ 0, 1, /* NOR */  1, -1, -1 )
	VB( /* POS */  1, -1,  1, /* TEX */ 0, 0, /* NOR */  1, -1,  1 )
	VB( /* POS */ -1, -1,  1, /* TEX */ 1, 0, /* NOR */ -1, -1,  1 )

	// LEFT FACE
	VB( /* POS */ -1, -1,  1, /* TEX */ 0, 1, /* NOR */ -1, -1,  1 )
	VB( /* POS */ -1,  1,  1, /* TEX */ 0, 0, /* NOR */ -1,  1,  1 )
	VB( /* POS */ -1,  1, -1, /* TEX */ 1, 0, /* NOR */ -1,  1, -1 )
	VB( /* POS */ -1, -1, -1, /* TEX */ 1, 1, /* NOR */ -1, -1, -1 )

	// RIGHT FACE
	VB( /* POS */  1, -1, -1, /* TEX */ 0, 1, /* NOR */  1, -1, -1 )
	VB( /* POS */  1,  1, -1, /* TEX */ 0, 0, /* NOR */  1,  1, -1 )
	VB( /* POS */  1,  1,  1, /* TEX */ 1, 0, /* NOR */  1,  1,  1 )
	VB( /* POS */  1, -1,  1, /* TEX */ 1, 1, /* NOR */  1, -1,  1 )

	// ------------ Index Buffer ---------------

	// FRONT FACE
	IB(0)  IB(1)  IB(2)
	IB(0)  IB(2)  IB(3)

	// BACK FACE
	IB(4)  IB(5)  IB(6)
	IB(4)  IB(6)  IB(7)

	// TOP FACE
	IB(8)  IB(9)  IB(10)
	IB(8)  IB(10) IB(11)

	// BOTTOM FACE
	IB(12) IB(13) IB(14)
	IB(12) IB(14) IB(15)

	// LEFT FACE
	IB(16) IB(17) IB(18)
	IB(16) IB(18) IB(19)

	// RIGHT FACE
	IB(20) IB(21) IB(22)
	IB(20) IB(22) IB(23)
}

void Skybox::draw() {
	context->PSSetShaderResources(0, 1, &skyboxTexture);
	context->PSSetSamplers(0, 1, &skyboxSampler);
	context->OMSetDepthStencilState(skyboxDepth, 0);
	context->RSSetState(skyboxRasterizer);

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