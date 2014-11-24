#pragma once

#include "Object3D.h"

namespace DDEngine
{
	class ModelObject : public Object3D<DirectX::VertexPositionNormalTangentTexture> {

	private:

		// Path to file containing a model data.
		std::string modelPath;

	public:
		ModelObject() : Object3D() { }
		ModelObject(const std::string& modelPath);
		~ModelObject();

	private:

		virtual void loadGeometry(std::vector<Mesh>& meshes) override;

	};
}