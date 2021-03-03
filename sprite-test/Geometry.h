#pragma once

#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QtGui/QMatrix4x4>
#include <QtGui/QMatrix3x3>

class Geometry
{
public:

	virtual void init() = 0;

	virtual void draw() = 0;

	virtual void draw_selection() = 0;

	virtual int line_intersect(float* p0, float* p1) = 0;

	void calculate3size(GLfloat* normals, int normals_size, GLfloat* vertices, int vertices_size);

	void set_matrix(const QMatrix4x4& matrix) { matrix_ = matrix; }

	void translate(float x, float y, float z) { matrix_.translate(x, y, z); }

	void rotate(float angle, float x, float y, float z = 0.0f) { matrix_.rotate(angle, x, y, z); }

	void scale(float x, float y, float z) { matrix_.scale(x, y, z); }

	void calc_center();

	void get_center(float& x, float& y, float& z) 
	{
		x = center_[0]; 
		y = center_[1]; 
		z = center_[2];
	}

	void get_center_offset(QVector3D& center)
	{
		center.setX(-center_[0]);
		center.setY(-center_[1]);
		center.setZ(-center_[2]);
	}

protected:

	QMatrix4x4 matrix_;


	float center_[3] = { 0, 0, 0 };
};

