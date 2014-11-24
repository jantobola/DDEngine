#include "AbstractObject.h"
#include "DDEUtils.h"
#include "ConstantBuffers.h"
#include "ShaderHolder.h"

using namespace DirectX;
using namespace std;

DDEngine::AbstractObject::AbstractObject()
{
	resetTransformations();
	createEmptyMaterials();
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

void DDEngine::AbstractObject::addMaterial(const std::string& path, Texture::TextureType type, DirectX::XMINT4 coords /* = Texture::DEFAULT_COORDS */, unsigned int index /* = 0 */)
{
	ShaderResourceView* textureResource = TextureUtils::createTexture(path, *Ctx);
	if (!textureResource) return;

	if (materials.size() >= index + 1) {
		bool matchType = false;

		// switch existing texture
		for (size_t i = 0; i < materials[index].textures.size(); i++) {
			
			Material &m = materials[index];
			Texture &t = m.textures[i];

			if (t.type == type) {
				ShaderResourceView* old = t.texture;
				RELEASE(old);

				t.path = path;
				t.texture = textureResource;

				m.textureArray[t.type] = t.texture;
				m.coordsArray[t.type] = coords;
				matchType = true;
			}
		}

		// add new texture into material
		if (!matchType) {
			Material &material = materials[index];
			material.textures.push_back(Texture(path, textureResource, type));
			material.sortTextures();

			material.textureArray[type] = textureResource;
			material.coordsArray[type] = coords;
		}

	} else {

		if (type != Texture::TextureType::UNKNOWN) {
			Material material;
			material.index = materials.size();
			material.textures.push_back(Texture(path, textureResource, type));
			material.textureArray[type] = textureResource;
			material.coordsArray[type] = coords;
			materials.push_back(material);
		}
	}

}

void DDEngine::AbstractObject::addMaterial(const std::string& path, std::string type, unsigned int index /*= 0*/)
{
	vector<string> typeArr = StringUtils::split(type, '.');

	if (typeArr.size() == 1) {
		addMaterial(path, Texture::valueOf(type), Texture::DEFAULT_COORDS, index);
		return;
	}

	if (typeArr.size() == 2) {
		addMaterial(path, Texture::valueOf(typeArr[0]), Texture::coordsOf(typeArr[1]), index);
	}
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

void DDEngine::AbstractObject::updateMatrices(CB::WorldViewProjection& cb)
{
	XMStoreFloat4x4(&cb.world, getWorldMatrix_T());
	XMStoreFloat4x4(&cb.invTransWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, getWorldMatrix_T())));
	XMStoreFloat4x4(&cb.view, Ctx->camera->getViewMatrix_T());
	XMStoreFloat4x4(&cb.projection, Ctx->camera->getProjectionMatrix_T());
}

void DDEngine::AbstractObject::defineMaterials(CB::Light& cb, std::string cbName, UINT materialIndex, UINT slot, bool availableInVS /* = false */)
{
	if (materials.size() == 0)
		return;

	cb.ambient = materials[materialIndex].ambient;
	cb.diffuse = materials[materialIndex].diffuse;
	cb.specular = materials[materialIndex].specular;
	cb.shininess = materials[materialIndex].shininess;

	cb.enableDNSA.x = materials[materialIndex].textureArray[0] != nullptr ? 1 : 0;
	cb.enableDNSA.y = materials[materialIndex].textureArray[1] != nullptr ? 1 : 0;
	cb.enableDNSA.z = materials[materialIndex].textureArray[2] != nullptr ? 1 : 0;
	cb.enableDNSA.w = materials[materialIndex].textureArray[3] != nullptr ? 1 : 0;
	cb.enableEHSO.x = materials[materialIndex].textureArray[4] != nullptr ? 1 : 0;
	cb.enableEHSO.y = materials[materialIndex].textureArray[5] != nullptr ? 1 : 0;
	cb.enableEHSO.z = materials[materialIndex].textureArray[6] != nullptr ? 1 : 0;
	cb.enableEHSO.w = materials[materialIndex].textureArray[7] != nullptr ? 1 : 0;
	cb.enableDLR.x  = materials[materialIndex].textureArray[8] != nullptr ? 1 : 0;
	cb.enableDLR.y  = materials[materialIndex].textureArray[9] != nullptr ? 1 : 0;
	cb.enableDLR.z = materials[materialIndex].textureArray[10] != nullptr ? 1 : 0;

	for (size_t i = 0; i < Texture::NUM_TYPES; i++) {
		cb.coords[i] = materials[materialIndex].coordsArray[i];
	}

	Ctx->getShaderHolder()->updateConstantBufferPS(cbName, &cb, slot);

	if (materialsInVS) {
		Ctx->getShaderHolder()->updateConstantBufferVS(cbName, &cb, slot);
	}
}

void DDEngine::AbstractObject::defineMaterials(CB::Light& cb, std::string cbName, UINT slot, bool availableInVS /*= false*/)
{
	materialsCB = &cb;
	materialsCBName = cbName;
	materialsCBSlot = slot;
	materialsInVS = availableInVS;
}

void DDEngine::AbstractObject::createEmptyMaterials()
{
	for (size_t i = 0; i < Texture::NUM_TYPES; i++) {
		emptyMaterials[i] = NULL;
	}
}

void DDEngine::AbstractObject::setAmbient(unsigned int index, float r, float g, float b, float a)
{
	materials[index].ambient = XMFLOAT4(r / 255, g / 255, b / 255, a / 255);
}

void DDEngine::AbstractObject::setDiffuse(unsigned int index, float r, float g, float b, float a)
{
	materials[index].diffuse = XMFLOAT4(r / 255, g / 255, b / 255, a / 255);
}

void DDEngine::AbstractObject::setSpecular(unsigned int index, float r, float g, float b, float a)
{
	materials[index].specular = XMFLOAT4(r / 255, g / 255, b / 255, a / 255);
}

void DDEngine::AbstractObject::setShininess(unsigned int index, float s)
{
	materials[index].shininess = s;
}

DirectX::XMFLOAT3 DDEngine::AbstractObject::getWorldPosition()
{
	XMVECTOR rot;
	XMVECTOR scale;
	XMVECTOR transl;
	XMFLOAT3 position;
	XMMatrixDecompose(&scale, &rot, &transl, getWorldMatrix());
	XMStoreFloat3(&position, transl);
	return position;
}
