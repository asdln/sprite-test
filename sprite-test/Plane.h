#pragma once

#include "PrimitiveSet.h"


class Plane : public PrimitiveSet
{
public:

	Plane();

	~Plane();

	float size(){ return size_; }

protected:

	float size_ = 1.0;
};

