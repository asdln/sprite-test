#pragma once

#include "Geometry.h"
#include "Triangle.h"
#include "Plane.h"

class Pyramid : public Geometry
{
public:

	Pyramid();

	void init();

	void draw();

protected:

	Triangle l;
	Triangle r;
	Triangle f;
	Triangle b;

	Plane p;
};

