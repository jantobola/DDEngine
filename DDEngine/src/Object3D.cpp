#include "Object3D.h"
#include "D3DUtils.h"
#include "DDEUtils.h"

using namespace DDEngine;
using namespace std;
using namespace DirectX;

Object3D::Object3D() {
	isHidden = false;
	setPrimitiveTopology(Object3D::TRIANGLE_LIST);
	XMMATRIX identityMatrix = XMMATRIX(XMMatrixIdentity());
	XMStoreFloat4x4(&worldMatrix, identityMatrix);
	XMStoreFloat4x4(&translationMatrix, identityMatrix);
	XMStoreFloat4x4(&rotationMatrix, identityMatrix);
	XMStoreFloat4x4(&scaleMatrix, identityMatrix);

	vertexBuffer = NULL;
	indexBuffer = NULL;
}

Object3D::~Object3D () {

}

HRESULT Object3D::registerObject(ID3D11Device* device, ID3D11DeviceContext* context) {
	HRESULT result = S_OK;

	this->device = device;
	this->context = context;

	loadGeometry();

	result = DXUtils::createVertexBuffer(device, &vertices, &vertexBuffer);
	result = DXUtils::createIndexBuffer(device, &indices, &indexBuffer);

	verticesSize = vertices.size();
	indicesSize = indices.size();

	vertices.clear();
	indices.clear();

	return result;
}

void Object3D::draw() {
	if(!isHidden) {
		UINT stride = sizeof( Vertex );
		UINT offset = 0;

		context->IASetPrimitiveTopology(this->primitiveTopology);
		context->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(indicesSize, 0, 0);
	}
}

void Object3D::setPrimitiveTopology( Object3D::PrimitiveTopology primitiveTopology ) {
	switch (primitiveTopology) {

	case TRIANGLE_LIST:
		this->primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;

	case TRIANGLE_STRIP:
		this->primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;

	default:
		break;
	}
}

int Object3D::getIndexCount() {
	return indicesSize;
}

int Object3D::getVertexCount() {
	return verticesSize;
}

XMMATRIX Object3D::getWorldMatrix() {
	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);
	world = XMLoadFloat4x4(&rotationMatrix) * XMLoadFloat4x4(&scaleMatrix) * XMLoadFloat4x4(&translationMatrix);
	XMStoreFloat4x4(&worldMatrix, world);
	return world;
}

void Object3D::releaseBuffers() {
	RELEASE( vertexBuffer )
	RELEASE( indexBuffer )
}

void Object3D::rotateX(float x) {
	float radians = XMConvertToRadians(x); 
	XMMATRIX transform = XMLoadFloat4x4(&rotationMatrix) * XMMatrixRotationX(radians);
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void Object3D::rotateY( float y ) {
	float radians = XMConvertToRadians(y); 
	XMMATRIX transform = XMLoadFloat4x4(&rotationMatrix) * XMMatrixRotationY(radians);
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void Object3D::rotateZ( float z ) {
	float radians = XMConvertToRadians(z); 
	XMMATRIX transform = XMLoadFloat4x4(&rotationMatrix) * XMMatrixRotationZ(radians);
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void Object3D::rotate( float x, float y, float z ) {
	float radiansX = XMConvertToRadians(x);
	float radiansY = XMConvertToRadians(y);
	float radiansZ = XMConvertToRadians(z); 
	XMMATRIX transform = XMMatrixRotationX(radiansX) * XMMatrixRotationY(radiansY) * XMMatrixRotationZ(radiansZ);
	transform = XMLoadFloat4x4(&rotationMatrix) * transform;
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void Object3D::translate( float x, float y, float z ) {
	XMMATRIX transform = XMLoadFloat4x4(&translationMatrix) * XMMatrixTranslation(x, y, z);
	XMStoreFloat4x4(&translationMatrix, transform);
}

void Object3D::scale( float scale ) {
	XMMATRIX transform = XMLoadFloat4x4(&scaleMatrix) * XMMatrixScaling(scale, scale, scale);
	XMStoreFloat4x4(&scaleMatrix, transform);
}

void Object3D::scale( float x, float y, float z ) {
	XMMATRIX transform = XMLoadFloat4x4(&scaleMatrix) * XMMatrixScaling(x, y, z);
	XMStoreFloat4x4(&scaleMatrix, transform);
}

void Object3D::setShaders( string vs, string ps, string il) {
	vertexShaderName = vs;
	pixelShaderName = ps;
	inputLayoutName = il;
}

std::string Object3D::getVSName() {
	return this->vertexShaderName;
}

std::string Object3D::getPSName() {
	return this->pixelShaderName;
}

std::string Object3D::getILName() {
	return this->inputLayoutName;
}

void Object3D::hide() {
	isHidden = true;
}

void Object3D::show() {
	isHidden = false;
}

void DDEngine::Object3D::VB(float x, float y, float z, float u, float v, float nx, float ny, float nz) {
	vertices.push_back(Vertex(x, y, z, u, v, nx, ny, nz));
}

void DDEngine::Object3D::IB(int index) {
	indices.push_back(index);
}
