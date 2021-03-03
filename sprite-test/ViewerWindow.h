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

	//QMatrix4x4 tempMatrix_;

	QQuaternion rotate_;

	QVector3D center_ = QVector3D(0, 0, 0);

	float distance_ = -10.0;

	float x0_;
	float y0_;

	float x1_;
	float y1_;

	float r_ = 0.8f;

private:

	GLuint Program = 0;
	GLuint vColorAttr_ = 0;

	GLuint tex;

	GLuint m_posAttr;
	GLuint m_colAttr;
	GLuint m_matrixUniform_mv;
	GLuint m_matrixUniform_p;

	QMatrix4x4 matrix_mv_;
	QMatrix4x4 matrix_p_;

	int m_frame;

	std::vector<std::shared_ptr<Geometry>> shapes_;
	std::vector<std::shared_ptr<LineSegment>> pick_lines_;
};
