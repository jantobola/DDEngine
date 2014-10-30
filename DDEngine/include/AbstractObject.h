#pragma once

#include "RenderContext.h"
#include "D3DUtils.h"
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

			// Flag indicates mesh is hidden or not.
			bool visibleFlag = true;

			// Model will be rendered as many times as a size of this container.
			// In most cases container will contain only one shader combination.
			std::vector<Shaders> shaders;

			std::vector<ShaderResourceView*> textures;

			// MATRICES

			DirectX::XMFLOAT4X4 worldMatrix;
			DirectX::XMFLOAT4X4 rotationMatrix;
			DirectX::XMFLOAT4X4 scaleMatrix;
			DirectX::XMFLOAT4X4 translationMatrix;

			// Reset all transofrmation matrices to identity matrices.
			void resetTransformations();

		private:

			// Internal cleaning function that releases all Direct3D
			// allcated buffers for a model.
			virtual void releaseBuffers() = 0;

		public:

			// Constructors & Destructor
			AbstractObject();
			virtual ~AbstractObject();

			bool selected = false;

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

			void setRotationMatrix(const DirectX::XMFLOAT4X4 rotationMatrix) { this->rotationMatrix = rotationMatrix; }
			void setScaleMatrix(const DirectX::XMFLOAT4X4 scaleMatrix) { this->scaleMatrix = scaleMatrix; }
			void setTranslationMatrix(const DirectX::XMFLOAT4X4 translationMatrix) { this->translationMatrix = translationMatrix; }

	};

}
