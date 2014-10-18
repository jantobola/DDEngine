#include "QuadObject.h"

using namespace DDEngine;

QuadObject::~QuadObject() {

}

void QuadObject::loadGeometry(std::vector<Mesh>& meshes) {

	Mesh mesh;

	// ---------- Vertex Buffer ------------

	mesh.VB( /* POS */ -1, +1, 0, /* TEX */ 0, 0, /* NOR */  0, 0, 0);
	mesh.VB( /* POS */ -1, -1, 0, /* TEX */ 1, 0, /* NOR */  0, 0, 0);
	mesh.VB( /* POS */ +1, -1, 0, /* TEX */ 1, 1, /* NOR */  0, 0, 0);
	mesh.VB( /* POS */ +1, +1, 0, /* TEX */ 0, 1, /* NOR */  0, 0, 0);

	// ------------ Index Buffer ---------------

	mesh.IB(1); mesh.IB(0); mesh.IB(2);
	mesh.IB(2); mesh.IB(0); mesh.IB(3);

	meshes.push_back(mesh);
}