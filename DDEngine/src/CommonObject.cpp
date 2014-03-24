#include "CommonObject.h"

using namespace DDEngine;

CommonObject::~CommonObject() {

}

void CommonObject::loadGeometry(std::vector<Vertex>* vertices, std::vector<DWORD>* indices) {
#define VB(x, y, z, u, v, nx, ny, nz) vertices->push_back(Vertex(x, y, z, u, v, nx, ny, nz));
#define IB(I) indices->push_back(I);

	// ---------- Vertex Buffer ------------

	// FRONT FACE
	VB( /* POS */ -1, -1, -1, /* TEX */ 0, 1, /* NOR */ -1, -1, -1 )
	VB( /* POS */ -1,  1, -1, /* TEX */ 0, 0, /* NOR */ -1,  1, -1 )
	VB( /* POS */  1,  1, -1, /* TEX */ 1, 0, /* NOR */  1,  1, -1 )
	VB( /* POS */  1, -1, -1, /* TEX */ 1, 1, /* NOR */  1, -1, -1 )

	// BACK FACE
	VB( /* POS */ -1, -1,  1, /* TEX */ 1, 1, /* NOR */ -1, -1,  1 )
	VB( /* POS */  1, -1,  1, /* TEX */ 0, 1, /* NOR */  1, -1,  1 )
	VB( /* POS */  1,  1,  1, /* TEX */ 0, 0, /* NOR */  1,  1,  1 )
	VB( /* POS */ -1,  1,  1, /* TEX */ 1, 0, /* NOR */ -1,  1,  1 )

	// TOP FACE
	VB( /* POS */ -1,  1, -1, /* TEX */ 0, 1, /* NOR */ -1,  1, -1 )
	VB( /* POS */ -1,  1,  1, /* TEX */ 0, 0, /* NOR */ -1,  1,  1 )
	VB( /* POS */  1,  1,  1, /* TEX */ 1, 0, /* NOR */  1,  1,  1 )
	VB( /* POS */  1,  1, -1, /* TEX */ 1, 1, /* NOR */  1,  1, -1 )

	// BOTTOM FACE
	VB( /* POS */ -1, -1, -1, /* TEX */ 1, 1, /* NOR */ -1, -1, -1 )
	VB( /* POS */  1, -1, -1, /* TEX */ 0, 1, /* NOR */  1, -1, -1 )
	VB( /* POS */  1, -1,  1, /* TEX */ 0, 0, /* NOR */  1, -1,  1 )
	VB( /* POS */ -1, -1,  1, /* TEX */ 1, 0, /* NOR */ -1, -1,  1 )

	// LEFT FACE
	VB( /* POS */ -1, -1,  1, /* TEX */ 0, 1, /* NOR */ -1, -1,  1 )
	VB( /* POS */ -1,  1,  1, /* TEX */ 0, 0, /* NOR */ -1,  1,  1 )
	VB( /* POS */ -1,  1, -1, /* TEX */ 1, 0, /* NOR */ -1,  1, -1 )
	VB( /* POS */ -1, -1, -1, /* TEX */ 1, 1, /* NOR */ -1, -1, -1 )

	// RIGHT FACE
	VB( /* POS */  1, -1, -1, /* TEX */ 0, 1, /* NOR */  1, -1, -1 )
	VB( /* POS */  1,  1, -1, /* TEX */ 0, 0, /* NOR */  1,  1, -1 )
	VB( /* POS */  1,  1,  1, /* TEX */ 1, 0, /* NOR */  1,  1,  1 )
	VB( /* POS */  1, -1,  1, /* TEX */ 1, 1, /* NOR */  1, -1,  1 )

	// ------------ Index Buffer ---------------

	// FRONT FACE
	IB(0)  IB(1)  IB(2)
	IB(0)  IB(2)  IB(3)

	// BACK FACE
	IB(4)  IB(5)  IB(6)
	IB(4)  IB(6)  IB(7)

	// TOP FACE
	IB(8)  IB(9)  IB(10)
	IB(8)  IB(10) IB(11)

	// BOTTOM FACE
	IB(12) IB(13) IB(14)
	IB(12) IB(14) IB(15)

	// LEFT FACE
	IB(16) IB(17) IB(18)
	IB(16) IB(18) IB(19)

	// RIGHT FACE
	IB(20) IB(21) IB(22)
	IB(20) IB(22) IB(23)

}