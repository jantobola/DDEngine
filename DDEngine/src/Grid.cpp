#include "Grid.h"

using namespace DDEngine;
using namespace DirectX;
using namespace std;

Grid::Grid() {
	Grid(0, 0);
}

Grid::Grid(int columns, int rows) {
	this->columns = columns;
	this->rows = rows;
}

Grid::~Grid() {

}

void Grid::loadGeometry(std::vector<Mesh>& meshes) {

	Mesh mesh;
	mesh.topology = PrimitiveTopology::TRIANGLE_STRIP;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < columns; c++) {
			mesh.VB(VertexPositionNormal(
				XMFLOAT3(float(c) / (columns - 1), float(r) / (rows - 1), 0),
				XMFLOAT3(0, 0, 0)
			));
		}
	}

	for (int r = 0; r < rows - 1; r++) {
		if (r % 2) {
			for (int c = columns - 1; c >= 0; c--) {
				mesh.IB((r + 1) * columns + c);
				mesh.IB(r * columns + c);
			}
			mesh.IB((r + 1) * columns);
			mesh.IB((r + 1) * columns);
		} else {
			for (int c = 0; c < columns; c++) {
				mesh.IB(r * columns + c);
				mesh.IB((r + 1) * columns + c);
			}
			mesh.IB((r + 2) * columns - 1);
			mesh.IB((r + 2) * columns - 1);
		}
	}
	
	meshes.push_back(mesh);
}

GridInfo Grid::getInfo() {
	GridInfo info;

	info.columns = columns;
	info.rows = rows;

	return info;
}
