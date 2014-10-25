#include "ModelObject.h"
#include "DDEUtils.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <DirectXTex/DirectXTex.h>

using namespace DDEngine;
using namespace Assimp;
using namespace DirectX;

DDEngine::ModelObject::ModelObject(const std::string& modelPath) : Object3D()
{
	this->modelPath = modelPath;
}

ModelObject::~ModelObject() {
	
}

void DDEngine::ModelObject::loadGeometry(std::vector<Mesh>& meshes)
{
	Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded |
		aiProcess_FlipUVs |
		aiProcess_SortByPType |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph |
		aiProcess_FixInfacingNormals
		);

	if (!scene) {
		Win32Utils::showMessage("Error during loading object.", importer.GetErrorString());
		return;
	}

	const aiVector3D nullVec(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[i];
		Mesh ddeMesh;

		for (size_t j = 0; j < mesh->mNumVertices; j++) {

			const aiVector3D* pos = &(mesh->mVertices[j]);
			const aiVector3D* nor = mesh->HasNormals() ? &(mesh->mNormals[j]) : &nullVec;
			const aiVector3D* tex = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][j]) : &nullVec;

			ddeMesh.VB(
				pos->x, pos->y, pos->z,
				tex->x, tex->y,
				nor->x, nor->y, nor->z
			);

			ddeMesh.materialIndex = mesh->mMaterialIndex;

		}

		for (size_t j = 0; j < mesh->mNumFaces; j++) {

			const aiFace& face = mesh->mFaces[j];

			switch (face.mNumIndices) {
				case 1: ddeMesh.topology = PrimitiveTopology::POINT_LIST; break;
				case 2: ddeMesh.topology = PrimitiveTopology::LINE_LIST; break;
				case 3: ddeMesh.topology = PrimitiveTopology::TRIANGLE_LIST; break;
				default: ddeMesh.topology = PrimitiveTopology::UNDEFINED; break;
			}

			for (size_t index = 0; index < face.mNumIndices; index++) {
				ddeMesh.IB(face.mIndices[index]);
			}

		}

		meshes.push_back(ddeMesh);
	}

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		const aiMaterial* material = scene->mMaterials[i];
		ShaderResourceView* texture = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string path = Path.data;
				std::size_t ext = path.find(".tga");
				ScratchImage image;
				HRESULT hr = -1;

				if (ext != std::string::npos) {
					hr = LoadFromTGAFile(StringUtils::toWstring("res/models/" + path).c_str(), nullptr, image);
				}
				else {
					texture = TextureUtils::createTexture("res/models/" + path, *Ctx);
				}

				if (SUCCEEDED(hr)) {
					hr = CreateShaderResourceView(Ctx->device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &texture);
				}

			}
		
		}

		if(texture) {
			textures.push_back(texture);
		} else {
			textures.push_back(TextureUtils::createTexture("res/textures/transparent.png", *Ctx));
		}
	}

}