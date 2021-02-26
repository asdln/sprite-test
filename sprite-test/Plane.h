#pragma once

#include "Geometry.h"


class Plane : public Geometry
{
public:

	Plane();

	~Plane();

	float size(){ return size_; }

protected:

	float size_ = 1.0;
};

