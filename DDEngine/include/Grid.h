#pragma once

#include "Object3D.h"

namespace DDEngine
{
struct GridInfo {
	int columns;
	int rows;
};

class Grid : public Object3D<DirectX::VertexPositionNormal> {

	public:
		Grid();
		Grid(int columns, int rows);
		~Grid();

	private:

		int columns;
		int rows;

		virtual void loadGeometry(std::vector<Mesh>& meshes) override;

	public:
		GridInfo getInfo();

};
}