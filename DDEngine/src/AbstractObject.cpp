#include "AbstractObject.h"

using namespace DirectX;

DDEngine::AbstractObject::AbstractObject()
{
	resetTransformations();
}

DDEngine::AbstractObject::~AbstractObject()
{

}

void DDEngine::AbstractObject::resetTransformations()
{
	XMMATRIX identityMatrix = XMMATRIX(XMMatrixIdentity());
	XMStoreFloat4x4(&worldMatrix, identityMatrix);
	XMStoreFloat4x4(&translationMatrix, identityMatrix);
	XMStoreFloat4x4(&rotationMatrix, identityMatrix);
	XMStoreFloat4x4(&scaleMatrix, identityMatrix);
}

void DDEngine::AbstractObject::addShaderCombination(std::string name, std::string vsName, std::string psName, std::string ilName, bool active /*= true*/)
{
	shaders.push_back(Shaders(name, vsName, psName, ilName, active));
}

void DDEngine::AbstractObject::enableShaderCombination(std::string name)
{
	for (Shaders shaderCombination : shaders) {
		if (shaderCombination.name == name)
			shaderCombination.active = true;
	}
}

void DDEngine::AbstractObject::disableShaderCombination(std::string name)
{
	for (Shaders shaderCombination : shaders) {
		if (shaderCombination.name == name)
			shaderCombination.active = false;
	}
}

void DDEngine::AbstractObject::addTexture(const std::string& path)
{
	ShaderResourceView* texture = TextureUtils::createTexture(path, *Ctx);
	textures.push_back(texture);
}

void DDEngine::AbstractObject::rotateX(float x)
{
	float radians = XMConvertToRadians(x);
	XMMATRIX transform = XMLoadFloat4x4(&rotationMatrix) * XMMatrixRotationX(radians);
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void DDEngine::AbstractObject::rotateY(float y)
{
	float radians = XMConvertToRadians(y);
	XMMATRIX transform = XMLoadFloat4x4(&rotationMatrix) * XMMatrixRotationY(radians);
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void DDEngine::AbstractObject::rotateZ(float z)
{
	float radians = XMConvertToRadians(z);
	XMMATRIX transform = XMLoadFloat4x4(&rotationMatrix) * XMMatrixRotationZ(radians);
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void DDEngine::AbstractObject::rotate(float x, float y, float z)
{
	float radiansX = XMConvertToRadians(x);
	float radiansY = XMConvertToRadians(y);
	float radiansZ = XMConvertToRadians(z);
	XMMATRIX transform = XMMatrixRotationX(radiansX) * XMMatrixRotationY(radiansY) * XMMatrixRotationZ(radiansZ);
	transform = XMLoadFloat4x4(&rotationMatrix) * transform;
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void DDEngine::AbstractObject::scale(float x, float y, float z)
{
	XMMATRIX transform = XMLoadFloat4x4(&scaleMatrix) * XMMatrixScaling(x, y, z);
	XMStoreFloat4x4(&scaleMatrix, transform);
}

void DDEngine::AbstractObject::scale(float xyz)
{
	XMMATRIX transform = XMLoadFloat4x4(&scaleMatrix) * XMMatrixScaling(xyz, xyz, xyz);
	XMStoreFloat4x4(&scaleMatrix, transform);
}

void DDEngine::AbstractObject::translate(float x, float y, float z)
{
	XMMATRIX transform = XMLoadFloat4x4(&translationMatrix) * XMMatrixTranslation(x, y, z);
	XMStoreFloat4x4(&translationMatrix, transform);
}

const DirectX::XMMATRIX DDEngine::AbstractObject::getWorldMatrix()
{
	XMMATRIX world = XMLoadFloat4x4(&scaleMatrix) * XMLoadFloat4x4(&rotationMatrix) * XMLoadFloat4x4(&translationMatrix);
	XMStoreFloat4x4(&worldMatrix, world);
	return world;
}

const DirectX::XMMATRIX DDEngine::AbstractObject::getWorldMatrix_T()
{
	return XMMatrixTranspose(getWorldMatrix());
}