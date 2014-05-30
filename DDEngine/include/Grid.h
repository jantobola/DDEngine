#pragma once

#include "Object3D.h"

namespace DDEngine
{
struct GridInfo {
	int columns;
	int rows;
};

class Grid : public Object3D {

	public:
		Grid();
		Grid(int columns, int rows);
		Grid(const Grid &copy) : Object3D(copy) { }
		~Grid();

	private:

		int columns;
		int rows;

		virtual void loadGeometry() override;

	public:
		GridInfo getInfo();

};
}