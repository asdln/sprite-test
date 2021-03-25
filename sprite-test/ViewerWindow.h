#pragma once

#include "openglwindow.h"


#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QMouseEvent>

#include <QtCore/qmath.h>
#include <QtGui/QOpenGLFunctions>
#include "teapot.h"
#include "Plane.h"
#include "Cube.h"
#include "funcs.h"
#include "Triangle.h"
#include "Pyramid.h"
#include "LineSegment.h"

#include "Sprite.h"

//! [1]
class ViewerWindow : public OpenGLWindow
{
public:
	ViewerWindow();
	~ViewerWindow();

	void initialize() override;
	void render() override;

	virtual void resizeEvent(QResizeEvent * event);

	virtual void mouseMoveEvent(QMouseEvent *ev);

	virtual void mousePressEvent(QMouseEvent *ev);

	virtual void mouseReleaseEvent(QMouseEvent *ev);

	virtual void keyPressEvent(QKeyEvent *ev);

	virtual void wheelEvent(QWheelEvent *ev);

protected:

	void init_program1();

	void init_program2();

	void create_geometrys();

	void calc_mv();

protected:

	//QMatrix4x4 tempMatrix_;

	QQuaternion rotate_;

	QVector3D center_ = QVector3D(0, 0, 0);

	QVector3D offset_ = QVector3D(0, 0, 0);

	float distance_ = -10.0;

	float x0_;
	float y0_;

	float x1_;
	float y1_;

	float ox_;
	float oy_;

	float r_ = 0.8f;

	bool is_rotate_ = true;

private:

	GLuint program_general = 0;
	GLuint m_matrixUniform_mv;
	GLuint m_matrixUniform_p;
	GLuint vColorAttr_ = 0;

	GLuint program_sprite = 0;
	GLuint sprite_matrix_uniform_p = 0;
	GLuint sprite_matrix_uniform_mv = 0;

	GLuint tex;


	std::shared_ptr<Sprite> sprite_;

	QMatrix4x4 matrix_mv_;
	QMatrix4x4 matrix_p_;

	int m_frame;

	std::vector<std::shared_ptr<Geometry>> shapes_;
	std::vector<std::shared_ptr<LineSegment>> pick_lines_;
};
