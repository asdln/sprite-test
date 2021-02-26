#include "Plane.h"

Plane::Plane()
{
	float size = 1.0;
	float updown_offset = 0.0;

	normal_count_ = 12;
	vertice_count_ = 12;
	indice_count_ = 4;

	vertices_ = new GLfloat[vertice_count_];
	normals_ = new GLfloat[normal_count_];
	indices_ = new GLushort[indice_count_];

	vertices_[0] = size * 0.5;
	vertices_[1] = updown_offset;
	vertices_[2] = size * 0.5;

	vertices_[3] = size * 0.5;
	vertices_[4] = updown_offset;
	vertices_[5] = size * -0.5;

	vertices_[6] = size * -0.5;
	vertices_[7] = updown_offset;
	vertices_[8] = size * -0.5;

	vertices_[9] = size * -0.5;
	vertices_[10] = updown_offset;
	vertices_[11] = size * 0.5;

	normals_[0] = 0;
	normals_[1] = 1;
	normals_[2] = 0;

	normals_[3] = 0;
	normals_[4] = 1;
	normals_[5] = 0;

	normals_[6] = 0;
	normals_[7] = 1;
	normals_[8] = 0;

	normals_[9] = 0;
	normals_[10] = 1;
	normals_[11] = 0;

	indices_[0] = 0;
	indices_[1] = 1;
	indices_[2] = 2;
	indices_[3] = 3;

	primitives_.emplace_back(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, 0);
}

Plane::~Plane()
{
	delete vertices_;
	delete normals_;
	delete indices_;
}