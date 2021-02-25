#include "Geometry.h"

void Geometry::calculate3size(GLfloat* normals, int normals_size, GLfloat* vertices, int vertices_size)
{
	if (!matrix_.isIdentity())
	{
		QMatrix3x3 normal_matrix = matrix_.normalMatrix();

		int normal_count = normals_size / sizeof(float) / 3;
		for (int i = 0; i < normal_count; i++)
		{
			normal_matrix * (normals + 3 * i);
		}

		for (int i = 0; i < vertices_size / sizeof(float) / 3; i++)
		{
			QVector4D vec(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2], 1.0);
			vec = matrix_ * vec;
			vertices[i * 3] = vec.x();
			vertices[i * 3 + 1] = vec.y();
			vertices[i * 3 + 2] = vec.z();
		}
	}
}

void Geometry::draw()
{
	glBindVertexArray(vert);

	for (auto primitive : primitives_)
	{
		glDrawElements(primitive.mode, primitive.count, primitive.type, (GLushort*)0 + primitive.offset);
	}
}