#pragma once

#include "Plane.h"
#include <QtGui/qmatrix4x4.h>

class Cube : public Geometry
{

public:

	void init();

	void draw();

protected:

	Plane l;
	Plane r;
	Plane t;
	Plane b;
	Plane front;
	Plane back;
};

