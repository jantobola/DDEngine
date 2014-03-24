#include "QuadObject.h"

using namespace DDEngine;

QuadObject::~QuadObject() {

}

void QuadObject::loadGeometry(std::vector<Vertex>* vertices, std::vector<DWORD>* indices) {
#define VB(x, y, z, u, v, nx, ny, nz) vertices->push_back(Vertex(x, y, z, u, v, nx, ny, nz));
#define IB(I) indices->push_back(I);

	// ---------- Vertex Buffer ------------

	VB( /* POS */ -1, +1, 0, /* TEX */ 1, 0, /* NOR */  0, 0, 0)
	VB( /* POS */ -1, -1, 0, /* TEX */ 0, 0, /* NOR */  0, 0, 0)
	VB( /* POS */ +1, -1, 0, /* TEX */ 0, 1, /* NOR */  0, 0, 0)
	VB( /* POS */ +1, +1, 0, /* TEX */ 1, 1, /* NOR */  0, 0, 0)

	// ------------ Index Buffer ---------------

	IB(1)  IB(0)  IB(2)
	IB(2)  IB(0)  IB(3)

}