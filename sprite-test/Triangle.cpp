#include "Triangle.h"

Triangle::Triangle()
{
	normal_count_ = 9;
	vertice_count_ = 9;
	indice_count_ = 3;

	vertices_ = new GLfloat[vertice_count_];
	normals_ = new GLfloat[normal_count_];
	indices_ = new GLushort[indice_count_];

	vertices_[0] = 0.0;
	vertices_[1] = 1.4142135623731 * 0.5;
	vertices_[2] = 0.0;

	vertices_[3] = -0.5;
	vertices_[4] = 0;
	vertices_[5] = 0;

	vertices_[6] = 0.5;
	vertices_[7] = 0;
	vertices_[8] = 0;

	normals_[0] = 0;
	normals_[1] = 0;
	normals_[2] = 1;

	normals_[3] = 0;
	normals_[4] = 0;
	normals_[5] = 1;

	normals_[6] = 0;
	normals_[7] = 0;
	normals_[8] = 1;

	indices_[0] = 0;
	indices_[1] = 1;
	indices_[2] = 2;

	primitives_.emplace_back(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
}


Triangle::~Triangle()
{
	delete vertices_;
	delete normals_;
	delete indices_;
}

