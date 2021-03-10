#include "ViewerWindow.h"



ViewerWindow::ViewerWindow()
	: m_frame(0)
{
}

ViewerWindow::~ViewerWindow()
{

}

void ViewerWindow::resizeEvent(QResizeEvent * event)
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

void ViewerWindow::mouseMoveEvent(QMouseEvent *ev)
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

void ViewerWindow::mousePressEvent(QMouseEvent *ev)
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

		auto pick_line = std::make_shared<LineSegment>(a, b);
		pick_lines_.push_back(pick_line);
		pick_line->init();

		for (auto shape : shapes_)
		{
			if (shape->line_intersect((float*)&a, (float*)&b) >= 0)
			{
				shape->get_center_offset(center_);
			}
		}
	}
}

void ViewerWindow::mouseReleaseEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::LeftButton)
	{

	}
}

void ViewerWindow::keyPressEvent(QKeyEvent *ev)
{
	rotate_ = QQuaternion();
	center_ = QVector3D(0.0, 0.0, 0.0);

	distance_ = -10.0;
	pick_lines_.clear();
}

void ViewerWindow::wheelEvent(QWheelEvent *ev)
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

//! [4]
void ViewerWindow::initialize()
{
	create_geometrys();

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

	ShaderInfo si[] = { { GL_VERTEX_SHADER, "PointSprite.vert" },{ GL_FRAGMENT_SHADER, "PointSprite.frag" },{ GL_NONE, NULL } };
	Program = LoadShaders(si);
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

void ViewerWindow::create_geometrys()
{
	auto teapot = std::make_shared<Teapot>();
	shapes_.emplace_back(teapot);
	teapot->scale(0.5, 0.5, 0.5);

	auto teapot2_ = std::make_shared<Teapot>();
	shapes_.emplace_back(teapot2_);
	teapot2_->translate(2, 1, -4);
	teapot2_->rotate(90, 1, 1, 1);
	teapot2_->scale(0.5, 0.5, 0.5);

	auto plane_ = std::make_shared<Plane>();
	shapes_.emplace_back(plane_);
 	plane_->scale(12, 0, 12);

	auto cube1_ = std::make_shared<Cube>();
	shapes_.emplace_back(cube1_);
	cube1_->translate(2, 1, 2);
	cube1_->rotate(45, 1, 1, 1);

	auto cube2_ = std::make_shared<Cube>();
	shapes_.emplace_back(cube2_);
	cube2_->translate(-2, 1.5, -2);
	cube2_->scale(1, 3, 1);

	auto triangle_ = std::make_shared<Triangle>();
	shapes_.emplace_back(triangle_);
	triangle_->translate(3, 0, -2);
	triangle_->scale(1, 2, 1);

	auto pyramid_ = std::make_shared<Pyramid>();
	shapes_.emplace_back(pyramid_);
	pyramid_->translate(0, 3, 0);
	pyramid_->rotate(90, 1, 0, 0);
	pyramid_->scale(1, 4, 1);

	auto pyramid2_ = std::make_shared<Pyramid>();
	shapes_.emplace_back(pyramid2_);
	pyramid2_->translate(-2, 0, 2);
	pyramid2_->scale(1, 2, 1);

	auto line_ = std::make_shared<LineSegment>(QVector3D(0, 0, 0), QVector3D(10.0, 10.0, 10.0));
	shapes_.emplace_back(line_);

	for (auto shape : shapes_)
	{
		shape->init();
	}
}

//! [5]
void ViewerWindow::render()
{
	//matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
// 	QMatrix4x4 matrix_mv;
// 	matrix_mv.translate(0, 0, -10);
// 
// 	//matrix_mv = tempMatrix_ * matrix_mv;
// 	matrix_mv = matrix_mv * tempMatrix_;
// 
// 	matrix_mv.translate(0, -1.57f, 0);

	QMatrix4x4 rotate_self;
	rotate_self.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

	QMatrix4x4 matrix_trans_to_eye;
	matrix_trans_to_eye.translate(0, 0, distance_);

	//matrix_mv = matrix_mv * tempMatrix_;

	QMatrix4x4 mat_rotate;
	mat_rotate.rotate(rotate_);

	// 	QMatrix4x4 matrix2;
	// 	matrix2.translate(0, -1.57f, 0);

	QMatrix4x4 matrix_trans_to_center;
	matrix_trans_to_center.translate(center_);

	QMatrix4x4 matrix_mv;
	matrix_mv = matrix_trans_to_eye * mat_rotate * matrix_trans_to_center;

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
		line->draw(m_matrixUniform_mv, matrix_mv_);
	}

	GLfloat colors[] = { 0.0, 0.0, 1.0, 1.0 };
	glUniform4fv(vColorAttr_, 1, colors);

	bool tag = 0;
	for (auto shape : shapes_)
	{
		std::shared_ptr<Teapot> teapot = std::dynamic_pointer_cast<Teapot>(shape);
		//使用标记，只旋转一个茶壶
		if (teapot && !tag) 
		{
			tag = 1;
			teapot->set_matrix_front(rotate_self);
		}
		
		shape->draw(m_matrixUniform_mv, matrix_mv_);
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