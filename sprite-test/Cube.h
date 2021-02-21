#pragma once

#include "Geometry.h"
#include <QtGui/qmatrix4x4.h>

class Cube : protected Geometry
{
protected:

	GLuint vert;
	GLuint vbo_vertex;
	GLuint vbo_normal;

	GLuint ebo;

	GLfloat vertices_[24];
	GLfloat normals_[12];
	GLushort indices_[4];

	QMatrix4x4 matrix_;
};

