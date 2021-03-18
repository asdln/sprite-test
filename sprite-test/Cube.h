#pragma once

#include "PrimitiveSetCollection.h"
#include <QtGui/qmatrix4x4.h>

class Cube : public PrimitiveSetCollection
{

public:

	void initImplemetation(GLuint program) override;

};

