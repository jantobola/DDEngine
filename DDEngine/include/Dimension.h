#pragma once

#include <windows.h>

namespace DDEngine
{
struct Dimension
{

	Dimension() {
		WIDTH = 0;
		HEIGHT = 0;
	}

	Dimension(UINT width, UINT height) {
		WIDTH = width;
		HEIGHT = height;
	}

	UINT WIDTH;
	UINT HEIGHT;

};
}