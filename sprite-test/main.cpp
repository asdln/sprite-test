/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "openglwindow.h"

#include <QtGui/QGuiApplication>
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
class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();
	~TriangleWindow();

    void initialize() override;
    void render() override;

	virtual void resizeEvent(QResizeEvent * event)
	{
		if (isInitialized())
		{
			QMatrix4x4 matrix_p;
			matrix_p.perspective(45.0f, (float)width() / height(), 1.0f, 1000.0f);
			glUniformMatrix4fv(m_matrixUniform_p, 1, GL_FALSE, matrix_p.data());
			matrix_p_ = matrix_p;
			glViewport(0, 0, width(), height());
		}
	}

	virtual void mouseMoveEvent(QMouseEvent *ev)
	{
		x0_ = x1_;
		y0_ = y1_;

		x1_ = getXnormalized(ev->pos().x(), 0, width() - 1);
		y1_ = getYnormalized(ev->pos().y(), 0, height() - 1);

		if (ev->buttons() & Qt::LeftButton)
		{
			float z0 = tb_project_to_sphere(r_, x0_, y0_);
			float z1 = tb_project_to_sphere(r_, x1_, y1_);

			QVector3D v0(x0_, y0_, z0);
			QVector3D v1(x1_, y1_, z1);

			v0.normalize();
			v1.normalize();

			QQuaternion quat_rotate = QQuaternion::rotationTo(v0, v1);
// 			QMatrix4x4 mat;
// 			mat.rotate(quat_rotate);

			rotate_ = quat_rotate * rotate_;

			//tempMatrix_ = mat * tempMatrix_;
			//tempMatrix_ = tempMatrix_ * mat;

		}
		else if (ev->buttons() & Qt::MiddleButton)
		{
			//center_ += QVector3D(x1_ - x0_, y1_ - y0_, 0.0);
		}
	}

	virtual void mousePressEvent(QMouseEvent *ev)
	{
		if (ev->buttons() & Qt::LeftButton)
		{
			x0_ = getXnormalized(ev->pos().x(), 0, width() - 1);
			y0_ = getYnormalized(ev->pos().y(), 0, height() - 1);

			x1_ = getXnormalized(ev->pos().x(), 0, width() - 1);
			y1_ = getYnormalized(ev->pos().y(), 0, height() - 1);
		}
		else if (ev->buttons() & Qt::RightButton)
		{
			float x = getXnormalized(ev->pos().x(), 0, width() - 1);
			float y = getYnormalized(ev->pos().y(), 0, height() - 1);

			QVector4D p1(x, y, 1.0, 1.0);
			QVector4D p2(x, y, -1.0, 1.0);

			QVector4D vec1 = matrix_mv_.inverted() * matrix_p_.inverted() * p1;
			QVector4D vec2 = matrix_mv_.inverted() * matrix_p_.inverted() * p2;

			QVector3D a;
			QVector3D b;

			a.setX(vec1.x() / vec1.w());
			a.setY(vec1.y() / vec1.w());
			a.setZ(vec1.z() / vec1.w());

			b.setX(vec2.x() / vec2.w());
			b.setY(vec2.y() / vec2.w());
			b.setZ(vec2.z() / vec2.w());

			for (auto shape : shapes_)
			{
				shape->line_intersect((float*)&a, (float*)&b);
			}

			auto pick_line = std::make_shared<LineSegment>(a, b);
			pick_lines_.push_back(pick_line);
			pick_line->init();
		}
	}

	virtual void mouseReleaseEvent(QMouseEvent *ev)
	{
		if (ev->button() == Qt::LeftButton)
		{

		}

	}

	virtual void keyPressEvent(QKeyEvent *ev)
	{
		rotate_ = QQuaternion();
		distance_ = -10.0;
		pick_lines_.clear();

	}

	virtual void wheelEvent(QWheelEvent *ev)
	{
		if (ev->delta() < 0)
		{
			//center_.setZ(center_.z() + 1.0);
			distance_ += 1.0;
		}
		else
		{
			//center_.setZ(center_.z() - 1.0);
			distance_ -= 1.0;
		}

	}

protected:

	//QMatrix4x4 tempMatrix_;

	QQuaternion rotate_;

	//QVector3D center_ = QVector3D(0, 0, -10);

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

TriangleWindow::TriangleWindow()
    : m_frame(0)
{
}

TriangleWindow::~TriangleWindow()
{

}

//! [4]
void TriangleWindow::initialize()
{
 	auto teapot = std::make_shared<Teapot>();
 	shapes_.emplace_back(teapot);
 	auto teapot2_ = std::make_shared<Teapot>();
 	shapes_.emplace_back(teapot2_);

	auto plane_ = std::make_shared<Plane>();
	shapes_.emplace_back(plane_);

 	auto cube1_ = std::make_shared<Cube>();
 	shapes_.emplace_back(cube1_);
 	auto cube2_ = std::make_shared<Cube>();
 	shapes_.emplace_back(cube2_);
 
 	auto triangle_ = std::make_shared<Triangle>();
 	shapes_.emplace_back(triangle_);
 
 	auto pyramid_ = std::make_shared<Pyramid>();
 	shapes_.emplace_back(pyramid_);
 	auto pyramid2_ = std::make_shared<Pyramid>();
 	shapes_.emplace_back(pyramid2_);
 
 	auto line_ = std::make_shared<LineSegment>(QVector3D(0, 0, 0), QVector3D(10.0, 10.0, 10.0));
 	shapes_.emplace_back(line_);

	ShaderInfo si[] = { { GL_VERTEX_SHADER, "PointSprite.vert" },{ GL_FRAGMENT_SHADER, "PointSprite.frag" },{ GL_NONE, NULL } };
	Program = LoadShaders(si);

	cube1_->translate(2, 1, 2);
	cube1_->rotate(45, 1, 1, 1);

	cube2_->translate(-2, 1.5, -2);
	cube2_->scale(1, 3, 1);

	teapot->scale(0.5, 0.5, 0.5);
	plane_->scale(12, 0, 12);

	teapot2_->translate(2, 1, -4);
	teapot2_->rotate(90, 1, 1, 1);
	teapot2_->scale(0.5, 0.5, 0.5);

	triangle_->translate(3, 0, -2);
	triangle_->scale(1, 2, 1);

	pyramid_->translate(0, 3, 0);
	pyramid_->rotate(90, 1, 0, 0);
	pyramid_->scale(1, 4, 1);

	pyramid2_->translate(-2, 0, 2);
 	pyramid2_->scale(1, 2, 1);

	for (auto shape : shapes_)
	{
		shape->init();
	}

	float texData[] = { 1.0, 0.0, 0.0, 1.0,
						0.0, 1.0, 0.0, 1.0,
						0.0, 0.0, 1.0, 1.0,
						1.0, 1.0 ,0.0, 1.0 };

	GLuint PiexlUnpack;
	glGenBuffers(1, &PiexlUnpack);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PiexlUnpack);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(texData), texData, GL_STATIC_DRAW);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 2, 2);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2, 2, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
	glUseProgram(Program);
	//glEnable(GL_POINT_SPRITE);
	//glEnable(GL_PROGRAM_POINT_SIZE);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	vColorAttr_ = glGetUniformLocation(Program, "vVertColor");
	GLfloat colors[] = { 0.0, 0.0, 1.0, 1.0 };
	glUniform4fv(vColorAttr_, 1, colors);
	
	//glShadeModel(GL_SMOOTH);

	//GLboolean res = 1;
	//glGetBooleanv(GL_POINT_SPRITE, &res);

	m_posAttr = glGetAttribLocation(Program, "posAttr");
	//m_colAttr = m_program->attributeLocation("colAttr");
	m_matrixUniform_mv = glGetUniformLocation(Program, "matrix_mv");
	m_matrixUniform_p = glGetUniformLocation(Program, "matrix_p");

	QMatrix4x4 matrix_p;
	matrix_p.perspective(45.0f, (float)width() / height(), 1.0f, 1000.0f);
	glUniformMatrix4fv(m_matrixUniform_p, 1, GL_FALSE, matrix_p.data());
	matrix_p_ = matrix_p;

	QVector4D Ambient = QVector4D(0.2f, 0.2f, 0.2f, 1.0f);
	glUniform4fv(glGetUniformLocation(Program, "Ambient"), 1, (const GLfloat*)&Ambient);

	QVector3D LightColor(0.9, 0.9, 0.9);
	QVector3D LightDirection(0.0, 0.0, -2.0);
	LightDirection.normalize();
	glUniform3fv(glGetUniformLocation(Program, "LightColor"), 1, (const GLfloat*)&LightColor);
	glUniform3fv(glGetUniformLocation(Program, "LightDirection"), 1, (const GLfloat*)&LightDirection);

	QVector3D Eye(0.0f, 0.0f, 0.0f);
	QVector3D HalfVector = Eye - LightDirection;
	HalfVector.normalize();

	glUniform3fv(glGetUniformLocation(Program, "HalfVector"), 1, (const GLfloat*)&HalfVector);
	glUniform1f(glGetUniformLocation(Program, "Shininess"), 5.0f);
	glUniform1f(glGetUniformLocation(Program, "Strength"), 0.9f);

	glViewport(0, 0, width(), height());
}
//! [4]

//! [5]
void TriangleWindow::render()
{
	//matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
// 	QMatrix4x4 matrix_mv;
// 	matrix_mv.translate(0, 0, -10);
// 
// 	//matrix_mv = tempMatrix_ * matrix_mv;
// 	matrix_mv = matrix_mv * tempMatrix_;
// 
// 	matrix_mv.translate(0, -1.57f, 0);

	QMatrix4x4 matrix1;
	matrix1.translate(0, 0, distance_);

	//matrix_mv = matrix_mv * tempMatrix_;

	QMatrix4x4 mat_rotate;
	mat_rotate.rotate(rotate_);

// 	QMatrix4x4 matrix2;
// 	matrix2.translate(0, -1.57f, 0);

	QMatrix4x4 matrix_mv;
	matrix_mv = matrix1 * mat_rotate /** matrix2*/;

	glUseProgram(Program);

	glUniformMatrix4fv(m_matrixUniform_mv, 1, GL_FALSE, matrix_mv.data());
	matrix_mv_ = matrix_mv;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GLfloat colors2[] = { 1.0, 0.0, 0.0, 1.0 };
	glUniform4fv(vColorAttr_, 1, colors2);

	//先画选择集。因为深度pass为“大于”
	for (auto shape : shapes_)
	{
		shape->draw_selection();
	}

	for (auto line : pick_lines_)
	{
		line->draw();
	}

	GLfloat colors[] = { 0.0, 0.0, 1.0, 1.0 };
	glUniform4fv(vColorAttr_, 1, colors);

	for (auto shape : shapes_)
	{
		shape->draw();
	}

	//glBindVertexArray(vert);
	//glUniform1i(0, 0);
	//glUniform1i(glGetUniformLocation(Program, "tex"), 0);

	//glPointSize(60);
	
	//glDrawArrays(GL_POINTS, 0, 4);

	//glUseProgram(0);

    ++m_frame;
	requestUpdate();
}
//! [5]

// float calczn(float ze, float f, float n)
// {
// 	return ((f + n) / (f - n) * ze + 2.0 * f * n / (f - n)) / ze;
// }

//! [2]
int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);

	QSurfaceFormat format;
	format.setSamples(16);

// 	float zn_min = calczn(-2.8, 100.0, 0.1);
// 	float zn_max = calczn(-1.2, 100.0, 0.1);
// 
// 	float zn_min2 = calczn(-2.8, 2.0, 0.1);
// 	float zn_max2 = calczn(-1.2, 2.0, 0.1);


	//QSurfaceFormat::SwapBehavior swr = format.swapBehavior();
	//format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

	TriangleWindow window;
	window.setFormat(format);
	window.resize(640, 480);
	window.show();

	//window.setAnimating(true);

	return app.exec();
}
//! [2]