#include "Plane.h"

Plane::Plane()
{
	float size = 1.0;
	float updown_offset = 0.0;

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

void Plane::init()
{
	calculate3size(normals_, sizeof(normals_), vertices_, sizeof(vertices_));

	initializeOpenGLFunctions();

	glGenVertexArrays(1, &vert);
	glBindVertexArray(vert);

	glGenBuffers(1, &vbo_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 /*3*/, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals_), normals_, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);                           // create a vbo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);                       // activate vbo id to use
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);    // upload data to video card

	glBindVertexArray(0);
}
