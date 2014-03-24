#pragma once

#include "Object3D.h"

namespace DDEngine
{
class CommonObject : public Object3D {

public:
	CommonObject() : Object3D() { }
	CommonObject(const CommonObject &copy) : Object3D(copy) { }
	~CommonObject();

private:

	virtual void loadGeometry(std::vector<Vertex>* vertices, std::vector<DWORD>* indices) override;

};
}