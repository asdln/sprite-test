#include "PrimitiveSet.h"


#include "funcs.h"

PrimitiveSet::PrimitiveSet()
{
	select_mode_ = SelectMode::SELECT_TRIANGLE;
}

PrimitiveSet::~PrimitiveSet()
{
	if (vert_)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vert_);
	}

	if (vbo_vertex_)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vbo_vertex_);
	}

	if (vbo_normal_)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vbo_normal_);
	}

	if (ebo_)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &ebo_);
	}
}

void PrimitiveSet::initImplemetation(GLuint program)
{
	program_ = program;
	//calculate3size(normals_, normal_count_, vertices_, vertice_count_);

	glGenVertexArrays(1, &vert_);
	glBindVertexArray(vert_);

	glGenBuffers(1, &vbo_vertex_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex_);
	glBufferData(GL_ARRAY_BUFFER, vertice_count_ * sizeof(GLfloat), vertices_, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 /*3*/, (void*)0);
	glEnableVertexAttribArray(0);

	if (normals_)
	{
		glGenBuffers(1, &vbo_normal_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal_);
		glBufferData(GL_ARRAY_BUFFER, normal_count_ * sizeof(GLfloat), normals_, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
	}

	glGenBuffers(1, &ebo_);                           // create a vbo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);                       // activate vbo id to use
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_count_ * sizeof(GLushort), indices_, GL_STATIC_DRAW);    // upload data to video card

	glBindVertexArray(0);

	if (select_mode_ == SelectMode::SELECT_TRIANGLE)
	{
		glGenVertexArrays(1, &vao_select_triangle_);
		glBindVertexArray(vao_select_triangle_);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex_);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 /*3*/, (void*)0);
		glEnableVertexAttribArray(0);

		if (normals_)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo_normal_);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);
		}

		glGenBuffers(1, &ebo_select_triangle_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_select_triangle_);
		glBindVertexArray(0);
	}
}

void PrimitiveSet::local(float* vertex, float* out, QMatrix4x4 ma)
{
	QMatrix4x4 matrix_scale;
	matrix_scale.scale(scale_[0], scale_[1], scale_[2]);

	QMatrix4x4 matrix_local;
	matrix_local = matrix_ * matrix_front_ * matrix_scale;

	matrix_local = ma * matrix_local;

	QVector4D vec(vertex[0], vertex[1], vertex[2], 1.0);
	vec = matrix_local * vec;

	out[0] = vec.x() / vec.w();
	out[1] = vec.y() / vec.w();
	out[2] = vec.z() / vec.w();
}

void PrimitiveSet::drawImplemetation(GLuint uniform_mv, const QMatrix4x4& mv)
{
// 	if (!pricked_primitives_.empty())
// 		return;

	QMatrix4x4 matrix_scale;
	matrix_scale.scale(scale_[0], scale_[1], scale_[2]);

	QMatrix4x4 matrix_local;
	matrix_local = mv * matrix_ * matrix_front_ * matrix_scale;
	glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, matrix_local.data());

	glBindVertexArray(vert_);

	for (auto primitive : primitives_)
	{
		glDrawElements(primitive.mode, primitive.count, primitive.type, (GLushort*)0 + primitive.offset);
	}

	glBindVertexArray(0);
}

void PrimitiveSet::draw_selection(GLuint uniform_mv, const QMatrix4x4& mv)
{
	QMatrix4x4 matrix_scale;
	matrix_scale.scale(scale_[0], scale_[1], scale_[2]);

	QMatrix4x4 matrix_local;
	matrix_local = mv * matrix_ * matrix_front_ * matrix_scale;
	glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, matrix_local.data());

	if (select_mode_ == SelectMode::SELECT_ELEMENT)
	{
		glBindVertexArray(vert_);
	}
	else if (select_mode_ == SelectMode::SELECT_TRIANGLE)
	{
		glBindVertexArray(vao_select_triangle_);
	}

	for (auto primitive : pricked_primitives_)
	{
		glDrawElements(primitive.mode, primitive.count, primitive.type, (GLushort*)0 + primitive.offset);
	}

	glBindVertexArray(0);
}

int PrimitiveSet::line_intersect(float* p0, float* p1, QMatrix4x4 ma)
{
	pricked_primitives_.clear();
	std::vector<GLushort> indices;

	int result = -1;

	for (const auto& primitive : primitives_)
	{
		switch (primitive.mode)
		{
		case GL_TRIANGLES:
		{
			size_t start_indice = primitive.offset;
			for (int i = 0; i < primitive.count / 3; i++)
			{
				float v1[3];
				float v2[3];
				float v3[3];

				local(vertices_ + indices_[start_indice + i * 3] * 3, v1, ma);
				local(vertices_ + indices_[start_indice + i * 3 + 1] * 3, v2, ma);
				local(vertices_ + indices_[start_indice + i * 3 + 2] * 3, v3, ma);

				int res = line_triangle_intersect(p0, p1, v1, v2, v3);
				if (res >= 0)
				{
					printf("tested \n");
					if (select_mode_ == SelectMode::SELECT_ELEMENT)
					{
						pricked_primitives_.emplace_back(primitive.mode, primitive.count, primitive.type, primitive.offset);
						break;
					}
					else if (select_mode_ == SelectMode::SELECT_TRIANGLE)
					{
 						indices.push_back(indices_[start_indice + i * 3]);
 						indices.push_back(indices_[start_indice + i * 3 + 1]);
 						indices.push_back(indices_[start_indice + i * 3 + 2]);
					}

					result = res;
				}
			}

			break;
		}
		case GL_TRIANGLE_FAN:
		{
			size_t start_indice = primitive.offset;
			for (int i = 2; i < primitive.count; i++)
			{
				float v1[3];
				float v2[3];
				float v3[3];

				local(vertices_ + indices_[start_indice] * 3, v1, ma);
				local(vertices_ + indices_[start_indice + i - 1] * 3, v2, ma);
				local(vertices_ + indices_[start_indice + i] * 3, v3, ma);

				int res = line_triangle_intersect(p0, p1, v1, v2, v3);
				if (res >= 0)
				{
					printf("tested \n");
					if (select_mode_ == SelectMode::SELECT_ELEMENT)
					{
						pricked_primitives_.emplace_back(primitive.mode, primitive.count, primitive.type, primitive.offset);
						break;
					}
					else if(select_mode_ == SelectMode::SELECT_TRIANGLE)
					{
						indices.push_back(indices_[start_indice]);
						indices.push_back(indices_[start_indice + i - 1]);
						indices.push_back(indices_[start_indice + i]);
					}

					result = res;
				}
			}

			break;
		}
		case GL_TRIANGLE_STRIP:
		{
			size_t start_indice = primitive.offset;
			for (int i = 2; i < primitive.count; i++)
			{
				float v1[3];
				float v2[3];
				float v3[3];

				local(vertices_ + indices_[start_indice + i - 2] * 3, v1, ma);
				local(vertices_ + indices_[start_indice + i - 1] * 3, v2, ma);
				local(vertices_ + indices_[start_indice + i] * 3, v3, ma);

				int res = line_triangle_intersect(p0, p1, v1, v2, v3);
				if (res >= 0)
				{
					printf("tested \n");
					if (select_mode_ == SelectMode::SELECT_ELEMENT)
					{
						pricked_primitives_.emplace_back(primitive.mode, primitive.count, primitive.type, primitive.offset);
						break;
					}
					else if (select_mode_ == SelectMode::SELECT_TRIANGLE)
					{
						indices.push_back(indices_[start_indice + i - 2]);
						indices.push_back(indices_[start_indice + i - 1]);
						indices.push_back(indices_[start_indice + i]);
					}

					result = res;
				}
			}

			break;
		}
		default:
			break;
		}
	}

	if (select_mode_ == SelectMode::SELECT_TRIANGLE)
	{
		pricked_primitives_.emplace_back(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

		//glBindVertexArray(vao_select_triangle_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_select_triangle_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glBindVertexArray(0);
	}

	return result;
}