#include "Geometry.h"
#include "funcs.h"

Geometry::Geometry()
{
}

Geometry::~Geometry()
{
	if (vert)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vert);
	}

	if (vbo_vertex)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vbo_vertex);
	}

	if (vbo_normal)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vbo_normal);
	}

	if (ebo)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &ebo);
	}
}

void Geometry::init()
{
	calculate3size(normals_, normal_count_, vertices_, vertice_count_);

	initializeOpenGLFunctions();

	glGenVertexArrays(1, &vert);
	glBindVertexArray(vert);

	glGenBuffers(1, &vbo_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, vertice_count_ * sizeof(GLfloat), vertices_, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 /*3*/, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, normal_count_ * sizeof(GLfloat), normals_, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);                           // create a vbo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);                       // activate vbo id to use
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_count_ * sizeof(GLfloat), indices_, GL_STATIC_DRAW);    // upload data to video card

	glBindVertexArray(0);
}

void Geometry::calculate3size(GLfloat* normals, int normals_size, GLfloat* vertices, int vertices_size)
{
	if (!matrix_.isIdentity())
	{
		QMatrix3x3 normal_matrix = matrix_.normalMatrix();

		int normal_count = normals_size / 3;
		for (int i = 0; i < normal_count; i++)
		{
			normal_matrix * (normals + 3 * i);
		}

		for (int i = 0; i < vertices_size / 3; i++)
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

int Geometry::line_intersect(float* p0, float* p1)
{
	for (const auto& primitive : primitives_)
	{
		switch (primitive.mode)
		{
		case GL_TRIANGLES:
		{
			size_t start_indice = primitive.offset / sizeof(GLushort);
			for (int i = 0; i < primitive.count / 3; i++)
			{
				int res = line_triangle_intersect(p0, p1, vertices_ + indices_[start_indice + i * 3] * 3, vertices_ + indices_[start_indice + i * 3 + 1] * 3, vertices_ + indices_[start_indice + i * 3 + 2] * 3);
				if (res >= 0)
				{
					printf("tested \n");

				}
			}
			
		}
		case GL_TRIANGLE_FAN:
		{

		}
		case GL_TRIANGLE_STRIP:
		{

		}
		default:
			break;
		}
	}

	return 0;
}