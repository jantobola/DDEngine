#pragma once

#include "RenderContext.h"
#include "D3DUtils.h"
#include "ConstantBuffers.h"
#include "Material.h"
#include <vector>
#include <windows.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <unordered_map>

namespace DDEngine {

	class AbstractObject
	{
		protected:

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

			RenderContext* Ctx;

			// Public name of a model to handle console command calls for this model.
			std::string modelName;
			unsigned int customIndex = 0;

			// Flag indicates mesh is hidden or not.
			bool visibleFlag = true;
			bool showNormals = false;
			bool solidWireframe = false;

			// Model will be rendered as many times as a size of this container.
			// In most cases container will contain only one shader combination.
			std::vector<Shaders> shaders;
			std::vector<Material> materials;

			// Materials Shader
			CB::Light defaultMaterialsCB;
			CB::Light* materialsCB = nullptr;
			std::string materialsCBName;
			UINT materialsCBSlot = 0;
			bool materialsInVS = false;
			ShaderResourceView* emptyMaterials[Texture::NUM_TYPES];

			// MATRICES

			DirectX::XMFLOAT4X4 worldMatrix;
			DirectX::XMFLOAT4X4 rotationMatrix;
			DirectX::XMFLOAT4X4 scaleMatrix;
			DirectX::XMFLOAT4X4 translationMatrix;

			// Reset all transofrmation matrices to identity matrices.
			void resetTransformations();
			void defineMaterials(CB::Light& cb, std::string cbName, UINT materialIndex, UINT slot, bool availableInVS = false);

		private:

			void createEmptyMaterials();

		public:

			// Constructors & Destructor
			AbstractObject();
			virtual ~AbstractObject();

			bool selected = false;

			// It will stop or start rendering of a model.
			inline void setVisible(bool isVisible) { this->visibleFlag = isVisible; }
			// Returns a flag mesh is visible or hidden.
			inline bool isVisible() { return visibleFlag; }

			inline void setShowNormals(bool showNormals) { this->showNormals = showNormals; }
			inline bool isShowNormals() { return showNormals; }

			inline void drawWireframe(bool wireframe) { this->solidWireframe = wireframe; }

			inline void setCustomIndex(unsigned int index) { customIndex = index; }
			inline unsigned int getCustomIndex() { return customIndex; }

			// Add (append) a shader combination.
			void addShaderCombination(std::string name, std::string vsName, std::string psName, std::string ilName, bool active = true);
			// Enables or disables rendering using a given shader combination.
			void enableShaderCombination(std::string name);
			void disableShaderCombination(std::string name);

			void addMaterial(const std::string& path, Texture::TextureType type, DirectX::XMINT4 coords = Texture::DEFAULT_COORDS, unsigned int index = 0);
			void addMaterial(const std::string& path, std::string type, unsigned int index = 0);

			void setAmbient(unsigned int index, float r, float g, float b, float a);
			void setDiffuse(unsigned int index, float r, float g, float b, float a);
			void setSpecular(unsigned int index, float r, float g, float b, float a);
			void setShininess(unsigned int index, float s);

			inline std::vector<Shaders>& getShaders() { return shaders; }
			inline const std::string getName() { return modelName; }

			void updateMatrices(CB::WorldViewProjection& cb);
			void defineMaterials(CB::Light& cb, std::string cbName, UINT slot, bool availableInVS = false);
			
			// Converts an engine structure into a Direct3D structure. This method should be
			// called in a pre-render stage.
			virtual void registerObject(const std::string& modelName, RenderContext& Ctx) = 0;

			// Finally draw a complete model.
			virtual void draw() = 0;

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

			inline void setRotationMatrix(const DirectX::XMFLOAT4X4 rotationMatrix) { this->rotationMatrix = rotationMatrix; }
			inline void setScaleMatrix(const DirectX::XMFLOAT4X4 scaleMatrix) { this->scaleMatrix = scaleMatrix; }
			inline void setTranslationMatrix(const DirectX::XMFLOAT4X4 translationMatrix) { this->translationMatrix = translationMatrix; }

			inline DirectX::XMFLOAT4X4 getRotationMatrix() { return rotationMatrix; }
			inline DirectX::XMFLOAT4X4 getScaleMatrix() { return scaleMatrix; }
			inline DirectX::XMFLOAT4X4 getTranslationMatrix() { return translationMatrix; }
			DirectX::XMFLOAT3 getWorldPosition();

	};

}
