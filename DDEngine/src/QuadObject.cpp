#include "QuadObject.h"

using namespace DDEngine;

QuadObject::~QuadObject() {

}

void QuadObject::loadGeometry() {

	// ---------- Vertex Buffer ------------

	VB( /* POS */ -1, +1, 0, /* TEX */ 0, 0, /* NOR */  0, 0, 0);
	VB( /* POS */ -1, -1, 0, /* TEX */ 1, 0, /* NOR */  0, 0, 0);
	VB( /* POS */ +1, -1, 0, /* TEX */ 1, 1, /* NOR */  0, 0, 0);
	VB( /* POS */ +1, +1, 0, /* TEX */ 0, 1, /* NOR */  0, 0, 0);

	// ------------ Index Buffer ---------------

	IB(1);  IB(0);  IB(2);
	IB(2);  IB(0);  IB(3);

}