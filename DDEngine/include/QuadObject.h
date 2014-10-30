#pragma once

#include "Object3D.h"

namespace DDEngine
{
	class QuadObject : public Object3D<DirectX::VertexPositionTexture> {

	public:
		QuadObject() : Object3D() { }
		QuadObject(const QuadObject &copy) : Object3D(copy) { }
		~QuadObject();

	private:

		virtual void loadGeometry(std::vector<Mesh>& meshes) override;

	};
}