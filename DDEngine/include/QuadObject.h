#pragma once

#include "Object3D.h"

namespace DDEngine
{
	class QuadObject : public Object3D {

	public:
		QuadObject() : Object3D() { }
		QuadObject(const QuadObject &copy) : Object3D(copy) { }
		~QuadObject();

	private:

		virtual void loadGeometry(std::vector<Vertex>* vertices, std::vector<DWORD>* indices) override;

	};
}