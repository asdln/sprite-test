#pragma once

#include "PrimitiveSet.h"
class LineSegment : public PrimitiveSet
{
public:

	LineSegment(QVector3D p1, QVector3D p2);
	~LineSegment();

	//void init();
};

