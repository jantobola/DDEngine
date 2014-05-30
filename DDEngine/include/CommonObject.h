#pragma once

#include "Object3D.h"

namespace DDEngine
{
class CubeObject : public Object3D {

public:
	CubeObject() : Object3D() { }
	CubeObject(const CubeObject &copy) : Object3D(copy) { }
	~CubeObject();

private:

	virtual void loadGeometry() override;

};
}