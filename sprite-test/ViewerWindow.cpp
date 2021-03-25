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
		glViewport(0, 0, width(), height());

		QMatrix4x4 matrix_p;
		matrix_p.perspective(45.0f, (float)width() / height(), 1.0f, 1000.0f);
		matrix_p_ = matrix_p;

		glUseProgram(program_general);
		glUniformMatrix4fv(m_matrixUniform_p, 1, GL_FALSE, matrix_p.data());

		glUseProgram(program_sprite);
		glUniformMatrix4fv(sprite_matrix_uniform_p, 1, GL_FALSE, matrix_p.data());
	}
}

void ViewerWindow::mouseMoveEvent(QMouseEvent *ev)
{
	x0_ = x1_;
	y0_ = y1_;

	QVector4D d4test(-center_, 1);
	d4test = matrix_p_ * matrix_mv_ * d4test;

	QVector3D d3test = d4test.toVector3DAffine();

	x1_ = getXnormalized(ev->pos().x(), 0, width() - 1) - d3test.x();
	y1_ = getYnormalized(ev->pos().y(), 0, height() - 1) - d3test.y();

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
}

void ViewerWindow::mousePressEvent(QMouseEvent *ev)
{
	if (ev->buttons() & Qt::LeftButton)
	{
		QVector4D d4test(-center_, 1);
		d4test = matrix_p_ * matrix_mv_ * d4test;

		QVector3D d3test = d4test.toVector3DAffine();

		x0_ = getXnormalized(ev->pos().x(), 0, width() - 1) - d3test.x();
		y0_ = getYnormalized(ev->pos().y(), 0, height() - 1) - d3test.y();

		x1_ = getXnormalized(ev->pos().x(), 0, width() - 1) - d3test.x();
		y1_ = getYnormalized(ev->pos().y(), 0, height() - 1) - d3test.y();
	}
	else if (ev->buttons() & Qt::RightButton)
	{
		float x = getXnormalized(ev->pos().x(), 0, width() - 1);
		float y = getYnormalized(ev->pos().y(), 0, height() - 1);

		QVector4D p1(x, y, 1.0, 1.0);
		QVector4D p2(x, y, -1.0, 1.0);

		QVector4D vec1 = matrix_mv_.inverted() * matrix_p_.inverted() * p1;
		QVector4D vec2 = matrix_mv_.inverted() * matrix_p_.inverted() * p2;

		QVector3D a = vec1.toVector3DAffine();
		QVector3D b = vec2.toVector3DAffine();

		auto pick_line = std::make_shared<LineSegment>(a, b);
		pick_lines_.push_back(pick_line);
		pick_line->init(program_general);

		for (auto shape : shapes_)
		{
			if (shape->line_intersect((float*)&a, (float*)&b, QMatrix4x4()) >= 0)
			{
				shape->get_center_offset(center_);
			}
		}
	}
	else if (ev->buttons() & Qt::MiddleButton)
	{
		ox_ = getXnormalized(ev->pos().x(), 0, width() - 1);
		oy_ = getYnormalized(ev->pos().y(), 0, height() - 1);
	}
}

void ViewerWindow::mouseReleaseEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::MidButton)
	{
		QMatrix4x4 matrix_trans_to_eye;
		matrix_trans_to_eye.translate(0, 0, distance_);
		
		QMatrix4x4 matrix_temp = matrix_p_ * matrix_trans_to_eye;

		QVector4D d4test(0, 0, 0, 1);
		d4test = matrix_temp * d4test;

		QVector3D d3test = d4test.toVector3DAffine();
		
		bool invertable = false;
		QMatrix4x4 matrix_invert = matrix_temp.inverted(&invertable);

		float x = getXnormalized(ev->pos().x(), 0, width() - 1);
		float y = getYnormalized(ev->pos().y(), 0, height() - 1);

		QVector4D v1(ox_, oy_, d3test.z(), 1);
		QVector4D v2(x, y, d3test.z(), 1);

		QVector4D v1_o = matrix_invert * v1;
		QVector4D v2_o = matrix_invert * v2;

		QVector3D v1_3 = v1_o.toVector3DAffine();
		QVector3D v2_3 = v2_o.toVector3DAffine();

		offset_ += v2_3 - v1_3;
	}
}

void ViewerWindow::keyPressEvent(QKeyEvent *ev)
{
	if (ev->key() == 82)
	{
		is_rotate_ = !is_rotate_;
	}
	else
	{
		rotate_ = QQuaternion();
		center_ = QVector3D(0.0, 0.0, 0.0);

		distance_ = -10.0;
		pick_lines_.clear();

		offset_ = QVector3D(0, 0, 0);
	}
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

void ViewerWindow::init_program1()
{
	ShaderInfo si[] = { { GL_VERTEX_SHADER, "general.vert" },{ GL_FRAGMENT_SHADER, "general.frag" },{ GL_NONE, NULL } };
	program_general = LoadShaders(si);
	glUseProgram(program_general);

	//glEnable(GL_CULL_FACE);

	vColorAttr_ = glGetUniformLocation(program_general, "vVertColor");
	GLfloat colors[] = { 0.0, 0.0, 1.0, 1.0 };
	glUniform4fv(vColorAttr_, 1, colors);

	//glShadeModel(GL_SMOOTH);

	//GLboolean res = 1;
	//glGetBooleanv(GL_POINT_SPRITE, &res);

	m_matrixUniform_mv = glGetUniformLocation(program_general, "matrix_mv");
	m_matrixUniform_p = glGetUniformLocation(program_general, "matrix_p");
	glUniformMatrix4fv(m_matrixUniform_p, 1, GL_FALSE, matrix_p_.data());

	QVector4D Ambient = QVector4D(0.2f, 0.2f, 0.2f, 1.0f);
	glUniform4fv(glGetUniformLocation(program_general, "Ambient"), 1, (const GLfloat*)&Ambient);

	QVector3D LightColor(0.9, 0.9, 0.9);
	QVector3D LightDirection(0.0, 0.0, -2.0);
	LightDirection.normalize();
	glUniform3fv(glGetUniformLocation(program_general, "LightColor"), 1, (const GLfloat*)&LightColor);
	glUniform3fv(glGetUniformLocation(program_general, "LightDirection"), 1, (const GLfloat*)&LightDirection);

	QVector3D Eye(0.0f, 0.0f, 0.0f);
	QVector3D HalfVector = Eye - LightDirection;
	HalfVector.normalize();

	glUniform3fv(glGetUniformLocation(program_general, "HalfVector"), 1, (const GLfloat*)&HalfVector);
	glUniform1f(glGetUniformLocation(program_general, "Shininess"), 5.0f);
	glUniform1f(glGetUniformLocation(program_general, "Strength"), 0.9f);
}

void ViewerWindow::init_program2()
{
	ShaderInfo si[] = { { GL_VERTEX_SHADER, "PointSprite.vert" },{ GL_FRAGMENT_SHADER, "PointSprite.frag" },{ GL_NONE, NULL } };
	program_sprite = LoadShaders(si);
	glUseProgram(program_sprite);

	sprite_matrix_uniform_mv = glGetUniformLocation(program_sprite, "matrix_mv");
	sprite_matrix_uniform_p = glGetUniformLocation(program_sprite, "matrix_p");
	glUniformMatrix4fv(sprite_matrix_uniform_p, 1, GL_FALSE, matrix_p_.data());
}

//! [4]
void ViewerWindow::initialize()
{
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
	glEnable(GL_DEPTH_TEST);

	matrix_p_.perspective(45.0f, (float)width() / height(), 1.0f, 1000.0f);

	init_program1();

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	init_program2();

	create_geometrys();

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
		shape->init(program_general);
	}

	GLfloat points[] = { 1.0, 1.0, 1.0, -1.5, 1.0, -1.5 };

	sprite_ = std::make_shared<Sprite>(points, 6);
	sprite_->init(program_sprite);
}

void ViewerWindow::calc_mv()
{
	QMatrix4x4 matrix_trans_to_eye;
	matrix_trans_to_eye.translate(0, 0, distance_);

	QMatrix4x4 matrix_offset;
	matrix_offset.translate(offset_);

	QMatrix4x4 mat_rotate;
	mat_rotate.rotate(rotate_);

	// 	QMatrix4x4 matrix2;
	// 	matrix2.translate(0, -1.57f, 0);

	QMatrix4x4 matrix_trans_to_center;
	matrix_trans_to_center.translate(center_);

	QMatrix4x4 matrix_mv;
	matrix_mv = matrix_trans_to_eye * matrix_offset * mat_rotate * matrix_trans_to_center;

	//glUseProgram(Program);

	//glUniformMatrix4fv(m_matrixUniform_mv, 1, GL_FALSE, matrix_mv.data());
	matrix_mv_ = matrix_mv;
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

	calc_mv();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glUseProgram(program_general);

	GLfloat colors2[] = { 1.0, 0.0, 0.0, 1.0 };
	glUniform4fv(vColorAttr_, 1, colors2);

	//先画选择集。因为深度pass为“大于”
	for (auto shape : shapes_)
	{
		shape->draw_selection(m_matrixUniform_mv, matrix_mv_);
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

	QMatrix4x4 rotate_self2;
	rotate_self2.rotate(-100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

	sprite_->set_matrix_front(rotate_self2);
	sprite_->draw(sprite_matrix_uniform_mv, matrix_mv_);

	//glBindVertexArray(vert);
	//glUniform1i(0, 0);
	//glUniform1i(glGetUniformLocation(Program, "tex"), 0);

	//glPointSize(60);

	//glDrawArrays(GL_POINTS, 0, 4);

	//glUseProgram(0);

	if(is_rotate_)
		++m_frame;

	requestUpdate();
}