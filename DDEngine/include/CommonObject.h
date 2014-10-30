#pragma once

#include "Object3D.h"

namespace DDEngine
{
	class CubeObject : public Object3D<DirectX::VertexPositionNormalTexture> {

public:
	CubeObject() : Object3D() { }
	CubeObject(const CubeObject &copy) : Object3D(copy) { }
	~CubeObject();

private:

	virtual void loadGeometry(std::vector<Mesh>& meshes) override;

};
}