#pragma once

#include "Geometry.h"
class LineSegment : public Geometry
{
public:

	LineSegment();
	LineSegment(QVector3D p1, QVector3D p2);
	~LineSegment();

	//void init();
};

