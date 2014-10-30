#include "QuadObject.h"

using namespace DDEngine;
using namespace DirectX;

QuadObject::~QuadObject() {

}

void QuadObject::loadGeometry(std::vector<Mesh>& meshes) {

#define VB(x, y, z, u, v) mesh.VB(VertexPositionTexture(XMFLOAT3(x, y, z), XMFLOAT2(u, v)))
#define IB(i) mesh.IB(i)

	Mesh mesh;

	// ---------- Vertex Buffer ------------

	VB( /* POS */ -1, +1, 0, /* TEX */ 0, 0);
	VB( /* POS */ -1, -1, 0, /* TEX */ 1, 0);
	VB( /* POS */ +1, -1, 0, /* TEX */ 1, 1);
	VB( /* POS */ +1, +1, 0, /* TEX */ 0, 1);

	// ------------ Index Buffer ---------------

	IB(1); IB(0); IB(2);
	IB(2); IB(0); IB(3);

	meshes.push_back(mesh);
}