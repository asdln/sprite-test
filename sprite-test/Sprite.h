#pragma once
#include "PrimitiveSet.h"
class Sprite :
	public PrimitiveSet
{
public:

	Sprite(GLfloat* vertices, int vertice_size);
	~Sprite();
};

