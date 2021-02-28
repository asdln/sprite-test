#include "Geometry.h"

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