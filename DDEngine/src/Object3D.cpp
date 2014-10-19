#include "Object3D.h"
#include "DDEUtils.h"

using namespace DirectX;

void DDEngine::Object3D::Mesh::VB(float x, float y, float z, float u, float v, float nx, float ny, float nz)
{
	this->vertices.push_back(Vertex(x, y, z, u, v, nx, ny, nz));
}

void DDEngine::Object3D::Mesh::IB(DWORD index)
{
	this->indices.push_back(index);
	numIndices++;
}

DDEngine::Object3D::Object3D()
{
	resetTransformations();
}

DDEngine::Object3D::~Object3D()
{
	releaseBuffers();
}

void DDEngine::Object3D::releaseBuffers()
{
	for (Mesh mesh : meshes) {
		RELEASE(mesh.vertexBuffer)
		RELEASE(mesh.indexBuffer)
	}

	for (ShaderResourceView* tex : textures) {
		RELEASE(tex)
	}

}

void DDEngine::Object3D::draw(const Mesh& mesh)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	Ctx->context->IASetPrimitiveTopology(translatePrimitiveTopology(mesh.topology));
	Ctx->context->IASetVertexBuffers(0, 1, &mesh.vertexBuffer, &stride, &offset);
	Ctx->context->IASetIndexBuffer(mesh.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (textures.size() > 0) {
		Ctx->setPSSampler(Ctx->commonStates->AnisotropicWrap(), 0);
		Ctx->setPSResource(textures[mesh.materialIndex], 0);
	}

	Ctx->context->DrawIndexed(mesh.numIndices, 0, 0);
}

void DDEngine::Object3D::draw()
{
	if (visibleFlag) {
		for (Mesh mesh : meshes) {
			draw(mesh);
		}
	}
}

void DDEngine::Object3D::addShaderCombination(std::string name, std::string vsName, std::string psName, std::string ilName, bool active /*= true*/)
{
	shaders.push_back(Shaders(name, vsName, psName, ilName, active));
}

void DDEngine::Object3D::enableShaderCombination(std::string name)
{
	for (Shaders shaderCombination : shaders) {
		if (shaderCombination.name == name)
			shaderCombination.active = true;
	}
}

void DDEngine::Object3D::disableShaderCombination(std::string name)
{
	for (Shaders shaderCombination : shaders) {
		if (shaderCombination.name == name)
			shaderCombination.active = false;
	}
}

void DDEngine::Object3D::registerObject(const std::string& modelName, RenderContext& Ctx)
{
	HRESULT result = S_OK;

	this->modelName = modelName;
	this->Ctx = &Ctx;

	loadGeometry(meshes);

	for (Mesh& mesh : meshes) {
		result = DXUtils::createVertexBuffer(Ctx.device, &mesh.vertices, &mesh.vertexBuffer);

		if (FAILED(result))
			Win32Utils::showFailMessage(result, "Vertex Buffer Error", "Error occurred during registering an object");

		result = DXUtils::createIndexBuffer(Ctx.device, &mesh.indices, &mesh.indexBuffer);

		if (FAILED(result))
			Win32Utils::showFailMessage(result, "Index Buffer Error", "Error occurred during registering an object");

		mesh.vertices.clear();
		mesh.vertices.shrink_to_fit();
		mesh.indices.clear();
		mesh.indices.shrink_to_fit();
	}

	Ctx.getRegisteredObjects().push_back(this);
}

void DDEngine::Object3D::addTexture(const std::string& path)
{
	ShaderResourceView* texture = TextureUtils::createTexture(path, *Ctx);
	textures.push_back(texture);
}

void DDEngine::Object3D::rotateX(float x)
{
	float radians = XMConvertToRadians(x);
	XMMATRIX transform = XMLoadFloat4x4(&rotationMatrix) * XMMatrixRotationX(radians);
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void DDEngine::Object3D::rotateY(float y)
{
	float radians = XMConvertToRadians(y);
	XMMATRIX transform = XMLoadFloat4x4(&rotationMatrix) * XMMatrixRotationY(radians);
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void DDEngine::Object3D::rotateZ(float z)
{
	float radians = XMConvertToRadians(z);
	XMMATRIX transform = XMLoadFloat4x4(&rotationMatrix) * XMMatrixRotationZ(radians);
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void DDEngine::Object3D::rotate(float x, float y, float z)
{
	float radiansX = XMConvertToRadians(x);
	float radiansY = XMConvertToRadians(y);
	float radiansZ = XMConvertToRadians(z);
	XMMATRIX transform = XMMatrixRotationX(radiansX) * XMMatrixRotationY(radiansY) * XMMatrixRotationZ(radiansZ);
	transform = XMLoadFloat4x4(&rotationMatrix) * transform;
	XMStoreFloat4x4(&rotationMatrix, transform);
}

void DDEngine::Object3D::scale(float x, float y, float z)
{
	XMMATRIX transform = XMLoadFloat4x4(&scaleMatrix) * XMMatrixScaling(x, y, z);
	XMStoreFloat4x4(&scaleMatrix, transform);
}

void DDEngine::Object3D::scale(float xyz)
{
	XMMATRIX transform = XMLoadFloat4x4(&scaleMatrix) * XMMatrixScaling(xyz, xyz, xyz);
	XMStoreFloat4x4(&scaleMatrix, transform);
}

void DDEngine::Object3D::translate(float x, float y, float z)
{
	XMMATRIX transform = XMLoadFloat4x4(&translationMatrix) * XMMatrixTranslation(x, y, z);
	XMStoreFloat4x4(&translationMatrix, transform);
}

const DirectX::XMMATRIX DDEngine::Object3D::getWorldMatrix()
{
	XMMATRIX world = XMLoadFloat4x4(&scaleMatrix) * XMLoadFloat4x4(&rotationMatrix) * XMLoadFloat4x4(&translationMatrix);
	XMStoreFloat4x4(&worldMatrix, world);
	return world;
}

const DirectX::XMMATRIX DDEngine::Object3D::getWorldMatrix_T()
{
	return XMMatrixTranspose(getWorldMatrix());
}

void DDEngine::Object3D::resetTransformations()
{
	XMMATRIX identityMatrix = XMMATRIX(XMMatrixIdentity());
	XMStoreFloat4x4(&worldMatrix, identityMatrix);
	XMStoreFloat4x4(&translationMatrix, identityMatrix);
	XMStoreFloat4x4(&rotationMatrix, identityMatrix);
	XMStoreFloat4x4(&scaleMatrix, identityMatrix);
}