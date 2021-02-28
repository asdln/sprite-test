#pragma once

#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QtGui/QMatrix4x4>
#include <QtGui/QMatrix3x3>
#include "Geometry.h"



struct Primitive
{
	GLushort mode;
	unsigned int count;
	GLushort type;
	unsigned int offset;

	Primitive(GLushort mode, unsigned int count, GLushort type, unsigned int offset)
	{
		this->mode = mode;
		this->count = count;
		this->type = type;
		this->offset = offset;
	}
};

class PrimitiveSet : public Geometry, public QOpenGLFunctions_4_3_Core
{
public:

	PrimitiveSet();

	~PrimitiveSet();

	virtual void init();

	virtual void draw();

	virtual int line_intersect(float* p0, float* p1);

protected:

	std::vector<Primitive> primitives_;

	GLuint vert = 0;
	GLuint vbo_vertex = 0;
	GLuint vbo_normal = 0;

	GLuint ebo = 0;

	GLfloat* vertices_ = nullptr;
	GLfloat* normals_ = nullptr;
	GLushort* indices_ = nullptr;

	size_t vertice_count_;
	size_t normal_count_;
	size_t indice_count_;

};
