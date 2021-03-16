#pragma once

#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QtGui/QMatrix4x4>
#include <QtGui/QMatrix3x3>

class Geometry
{
public:

	virtual void init() = 0;

	virtual void draw(GLuint, const QMatrix4x4&) = 0;

	virtual void draw_selection(GLuint uniform_mv, const QMatrix4x4& mv) = 0;

	virtual int line_intersect(float* p0, float* p1, QMatrix4x4 ma) = 0;

//	void calculate3size(GLfloat* normals, int normals_size, GLfloat* vertices, int vertices_size);

	void set_matrix(const QMatrix4x4& matrix) { matrix_ = matrix; }

	void get_matrix(QMatrix4x4& matrix) { matrix = matrix_; }

	void translate(float x, float y, float z) { matrix_.translate(x, y, z); }

	void rotate(float angle, float x, float y, float z = 0.0f) { matrix_.rotate(angle, x, y, z); }

	void scale(float x, float y, float z) { scale_[0] = x; scale_[1] = y;  scale_[2] = z; }

	void calc_center();

	void set_matrix_front(const QMatrix4x4& matrix) { matrix_front_ = matrix; }

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

	QMatrix4x4 matrix_front_;

	float scale_[3] = { 1, 1, 1 };

	float center_[3] = { 0, 0, 0 };
};

