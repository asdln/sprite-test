#pragma once

#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QtGui/QMatrix4x4>
#include <QtGui/QMatrix3x3>
#include "Geometry.h"

enum class SelectMode
{
	SELECT_ELEMENT,
	SELECT_TRIANGLE
};


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

	virtual void draw(GLuint uniform_mv, const QMatrix4x4& mv) override;

	virtual void draw_selection() override;

	virtual int line_intersect(float* p0, float* p1) override;

protected:

	SelectMode select_mode_ = SelectMode::SELECT_ELEMENT;

	std::vector<Primitive> primitives_;

	std::vector<Primitive> pricked_primitives_;

	GLuint vert_ = 0;
	GLuint vbo_vertex_ = 0;
	GLuint vbo_normal_ = 0;

	GLuint ebo_ = 0;

	GLuint vao_select_triangle_;

	GLuint ebo_select_triangle_;

	GLfloat* vertices_ = nullptr;
	GLfloat* normals_ = nullptr;
	GLushort* indices_ = nullptr;

	size_t vertice_count_;
	size_t normal_count_;
	size_t indice_count_;

};
