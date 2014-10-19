#pragma once

#include "Vertex.h"
#include <vector>
#include <windows.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include "D3DUtils.h"
#include <unordered_map>

namespace DDEngine
{
	/*
		This abstract class represents a drawable model.
	*/
	class Object3D {

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
				std::vector<Vertex> vertices;
				std::vector<DWORD> indices;

				// Number of indices and index of a material.
				unsigned int numIndices = 0;
				unsigned int materialIndex = 0;

				// Set default primitive topology. Each mesh may have a different topology.
				PrimitiveTopology topology = PrimitiveTopology::TRIANGLE_LIST;
				// pointers to Direct3D vertex and index buffer for a given mesh
				ID3D11Buffer *vertexBuffer, *indexBuffer = nullptr;

				// Helper methods for creating vertex and index structure.
				void VB(float x, float y, float z, float u, float v, float nx, float ny, float nz);
				void IB(DWORD index);
			};

			/*
				An envelope of shader combination.
			*/
			struct Shaders
			{

				Shaders(
					std::string name,
					std::string vsName,
					std::string psName,
					std::string ilName,
					bool active
				) :
					name(name),
					vsName(vsName),
					psName(psName),
					ilName(ilName),
					active(active)
				{ }

				~Shaders() { }

				// Indicates if engine should render a model using this shader combination.
				bool active = true;
				std::string name;

				// String reference to a engine Shader Holder.
				std::string vsName;
				std::string psName;
				std::string ilName;
			};

		private:

			// Flag indicates mesh is hidden or not.
			bool visibleFlag = true;

			// Internal cleaning function that releases all Direct3D
			// allcated buffers for a model.
			void releaseBuffers();

			// Reset all transofrmation matrices to identity matrices.
			void resetTransformations();

		protected:

			RenderContext* Ctx;

			// Public name of a model to handle console command calls for this model.
			std::string modelName;

			// Internal mesh structure for a model.
			std::vector<Mesh> meshes;

			// Model will be rendered as many times as a size of this container.
			// In most cases container will contain only one shader combination.
			std::vector<Shaders> shaders;

			std::vector<ShaderResourceView*> textures;

			// Load your geometry and other things in child classes from whatever you want.
			virtual void loadGeometry(std::vector<Mesh>& meshes) = 0;

			// Internal draw operation for a single mesh.
			virtual void draw(const Mesh& mesh);

			// MATRICES

			DirectX::XMFLOAT4X4 worldMatrix;
			DirectX::XMFLOAT4X4 rotationMatrix;
			DirectX::XMFLOAT4X4 scaleMatrix;
			DirectX::XMFLOAT4X4 translationMatrix;

		public:

			// Constructors & Destructor
			Object3D();
			virtual ~Object3D();

			// It will stop or start rendering of a model.
			void setVisible(bool isVisible) { this->visibleFlag = isVisible; }
			// Returns a flag mesh is visible or hidden.
			bool isVisible() { return visibleFlag; }

			// Add (append) a shader combination.
			void addShaderCombination(std::string name, std::string vsName, std::string psName, std::string ilName, bool active = true);
			// Enables or disables rendering using a given shader combination.
			void enableShaderCombination(std::string name);
			void disableShaderCombination(std::string name);

			void addTexture(const std::string& path);

			std::vector<Shaders>& getShaders() { return shaders; }
			const std::string getName() { return modelName; }

			// Converts an engine structure into a Direct3D structure. This method should be
			// called in a pre-render stage.
			virtual void registerObject(const std::string& modelName, RenderContext& Ctx);
			
			// Finally draw a complete model.
			virtual void draw();

			// TRANSFORMATIONS

			// Applies a rotation transformation on a model.
			void rotateX(float x);
			void rotateY(float y);
			void rotateZ(float z);
			void rotate(float x, float y, float z);

			// Applies a scale transformation on a model.
			void scale(float x, float y, float z);
			void scale(float xyz);

			// Applies a translation transformation on a model.
			void translate(float x, float y, float z);

			// Get model in world space.
			const DirectX::XMMATRIX getWorldMatrix();
			// Get transpose world space matrix
			const DirectX::XMMATRIX getWorldMatrix_T();

	};
}