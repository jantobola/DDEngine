#pragma once

#include "Vertex.h"
#include "AbstractObject.h"
#include "DDEUtils.h"
#include "ShaderHolder.h"
#include "ConstantBuffers.h"
#include <DirectXTK/VertexTypes.h>

namespace DDEngine
{
	/*
		This abstract class represents a drawable model.
	*/
	template <class T>
	class Object3D : public AbstractObject {

		protected:

			/*
				An internal representation of a single mesh. A complete model loaded from a file
				is composed from one or multiple meshes referencing different materials and textures.
			*/
			struct Mesh
			{
				// Pointers to vertices and indices engine structure.
				// These pointers should be deleted after Direct3D vertex and index buffer creation.
				// We don't want to hold a whole geometry of a mesh in a memory when this Direct3D does.
				std::vector<T> vertices;
				std::vector<DWORD> indices;

				// Number of indices and index of a material.
				unsigned int numIndices = 0;
				unsigned int materialIndex = 0;

				// Set default primitive topology. Each mesh may have a different topology.
				PrimitiveTopology topology = PrimitiveTopology::TRIANGLE_LIST;
				// pointers to Direct3D vertex and index buffer for a given mesh
				ID3D11Buffer *vertexBuffer, *indexBuffer = nullptr;

				// Helper methods for creating vertex and index structure.
				void VB(T vertex)
				{
					vertices.push_back(vertex);
				}
				
				void IB(DWORD index)
				{
					indices.push_back(index);
					numIndices++;
				}
			};

			// Internal mesh structure for a model.
			std::vector<Mesh> meshes;

			// Load your geometry and other things in child classes from whatever you want.
			virtual void loadGeometry(std::vector<Mesh>& meshes) = 0;

			// Internal draw operation for a single mesh.
			virtual void draw(const Mesh& mesh)
			{
				UINT stride = sizeof(T);
				UINT offset = 0;

				Ctx->context->IASetPrimitiveTopology(translatePrimitiveTopology(mesh.topology));
				Ctx->context->IASetVertexBuffers(0, 1, &mesh.vertexBuffer, &stride, &offset);
				Ctx->context->IASetIndexBuffer(mesh.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

				if (textures.size() > 0) {
					//TODO config flag if anisotropic is enabled
					Ctx->setPSSampler(Ctx->commonStates->AnisotropicWrap(), 0);
					Ctx->setPSResource(textures[mesh.materialIndex], 0);
				}

				Ctx->context->DrawIndexed(mesh.numIndices, 0, 0);
			}

		private:

			CB::GSMatrices cb_matrices;

		public:

			// Constructors & Destructor
			Object3D() { }
			virtual ~Object3D()
			{
				releaseBuffers();
			}
			
			virtual void registerObject(const std::string& modelName, RenderContext& Ctx) override
			{
				HRESULT result = S_OK;

				this->modelName = modelName;
				this->Ctx = &Ctx;

				loadGeometry(meshes);

				for (Mesh& mesh : meshes) {
					result = DXUtils::createVertexBuffer<T>(Ctx.device, &mesh.vertices, &mesh.vertexBuffer);

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
			
			
			virtual void draw() override
			{
				if (visibleFlag) {

					for (Mesh mesh : meshes) {
						draw(mesh);
					}

					// Draw some visual effect when object is selected
					if (selected) {
						for (Mesh mesh : meshes) {
							UINT stride = sizeof(T);
							UINT offset = 0;

							Ctx->context->IASetPrimitiveTopology(translatePrimitiveTopology(mesh.topology));
							Ctx->context->IASetVertexBuffers(0, 1, &mesh.vertexBuffer, &stride, &offset);
							Ctx->context->IASetIndexBuffer(mesh.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

							Ctx->context->RSSetState(Ctx->RSWiredCullBack);

							Ctx->getShaderHolder()->activatePS("DDEngine_PS_White");
							Ctx->context->DrawIndexed(mesh.numIndices, 0, 0);
						}

						Ctx->setRasterizerState(Ctx->currentRasterizerState);
					}

					if (showNormals) {
						for (Mesh mesh : meshes) {
							UINT stride = sizeof(T);
							UINT offset = 0;

							Ctx->context->IASetPrimitiveTopology(translatePrimitiveTopology(mesh.topology));
							Ctx->context->IASetVertexBuffers(0, 1, &mesh.vertexBuffer, &stride, &offset);
							Ctx->context->IASetIndexBuffer(mesh.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

							Ctx->context->RSSetState(Ctx->RSWiredCullNone);

							Ctx->getShaderHolder()->activateGS("DDEngine_GS_NormalVisualizer");
							Ctx->getShaderHolder()->activatePS("DDEngine_PS_Green");

							setCB(cb_matrices);
							XMStoreFloat4x4(&cb_matrices.invWorld, XMMatrixInverse(nullptr, getWorldMatrix_T()));
							XMStoreFloat4x4(&cb_matrices.invView, XMMatrixInverse(nullptr, Ctx->camera->getViewMatrix_T()));
							XMStoreFloat4x4(&cb_matrices.invProjection, XMMatrixInverse(nullptr, Ctx->camera->getProjectionMatrix_T()));
							cb_matrices.scale = getScaleMatrix();
							
							Ctx->getShaderHolder()->updateConstantBufferGS("DDEngine_Matrices", &cb_matrices, 0);
							Ctx->context->DrawIndexed(mesh.numIndices, 0, 0);
						}

						Ctx->setRasterizerState(Ctx->currentRasterizerState);
						Ctx->context->GSSetShader(nullptr, nullptr, 0);
					}
				}
			}

			// A cleaning function that releases all Direct3D
			// allcated buffers for a model.
			void releaseBuffers()
			{
				for (Mesh mesh : meshes) {
					RELEASE(mesh.vertexBuffer)
						RELEASE(mesh.indexBuffer)
				}

				for (ShaderResourceView* tex : textures) {
					RELEASE(tex)
				}
			}
			
	};
}