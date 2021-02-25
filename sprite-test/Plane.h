#pragma once

#include "Geometry.h"


class Plane : public Geometry
{
public:

	Plane();

	void init();

	float size(){ return size_; }

protected:

	GLfloat vertices_[12];
	GLfloat normals_[12];
	GLushort indices_[4];

	float size_ = 1.0;
};

