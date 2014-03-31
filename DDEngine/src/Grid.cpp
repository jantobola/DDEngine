#include "Grid.h"

using namespace DDEngine;
using namespace std;

Grid::Grid() {
	Grid(0, 0);
}

Grid::Grid(int columns, int rows) {
	Object3D::Object3D();
	setPrimitiveTopology(Object3D::TRIANGLE_STRIP);

	this->columns = columns;
	this->rows = rows;
}

Grid::~Grid() {

}

void Grid::loadGeometry(vector<Vertex>* vertices, vector<DWORD>* indices) {
#define VB(x, y, z, u, v, nx, ny, nz) vertices->push_back(Vertex(x, y, z, u, v, nx, ny, nz));
#define IB(I) indices->push_back(I);

// creates lines between degenerated triangles

// 	float mulX = (float) width / (float) columns;
// 	float mulY = (float) height / (float) rows;
// 
// 	for (int r = 0; r < rows; r++) {
// 		for (int c = 0; c < columns; c++) {
// 			VB( (float) c * mulX , 0, (float) (rows - r) * mulY, 0, 0, 1, 1, 1)
// 		}
// 	}
// 
// 	for (int r = 0; r < rows - 1; r++) {
// 		IB(r * columns);
// 		for (int c = 0; c < columns; c++) {
// 			IB(r * columns + c);
// 			IB((r + 1) * columns + c);
// 		}
// 		IB((r + 1) * columns + (columns - 1));
// 	}

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < columns; c++) {
			VB(float(c) / (columns - 1), float(r) / (rows - 1), 0, 0, 0, 0, 0, 0)
		}
	}

	for (int r = 0; r < rows - 1; r++) {
		if (r % 2) {
			for (int c = columns - 1; c >= 0; c--) {
				IB((r + 1) * columns + c);
				IB(r * columns + c);
			}
			IB((r + 1) * columns);
			IB((r + 1) * columns);
		} else {
			for (int c = 0; c < columns; c++) {
				IB(r * columns + c);
				IB((r + 1) * columns + c);
			}
			IB((r + 2) * columns - 1);
			IB((r + 2) * columns - 1);
		}
	}
	
}

GridInfo Grid::getInfo() {
	GridInfo info;

	info.columns = columns;
	info.rows = rows;

	return info;
}
