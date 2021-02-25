#pragma once

#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QtGui/QMatrix4x4>
#include <QtGui/QMatrix3x3>

inline QVector3D operator*(const QMatrix3x3& m, const QVector3D& v)
{
	
	QVector3D res;

	res.setX(m(0, 0) * v.x() + m(0, 1) * v.y() + m(0, 2) * v.z());
	res.setY(m(1, 0) * v.x() + m(1, 1) * v.y() + m(1, 2) * v.z());
	res.setZ(m(2, 0) * v.x() + m(2, 1) * v.y() + m(2, 2) * v.z());

	return res;
}

inline void operator*(const QMatrix3x3& m, GLfloat* v)
{
	float x = v[0];
	float y = v[1];
	float z = v[2];

	v[0] = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z;
	v[1] = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z;
	v[2] = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z;
}

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

class Geometry : protected QOpenGLFunctions_4_3_Core
{
public:

	virtual void init() {}

	virtual void draw();

	void calculate3size(GLfloat* normals, int normals_size, GLfloat* vertices, int vertices_size);

	void set_matrix(const QMatrix4x4& matrix) { matrix_ = matrix; }

	void translate(float x, float y, float z){ matrix_.translate(x, y, z);}

	void rotate(float angle, float x, float y, float z = 0.0f){ matrix_.rotate(angle, x, y, z); }

	void scale(float x, float y, float z){ matrix_.scale(x, y, z); }

protected:

	std::vector<Primitive> primitives_;

	QMatrix4x4 matrix_;

	GLuint vert;
	GLuint vbo_vertex;
	GLuint vbo_normal;

	GLuint ebo;
};

