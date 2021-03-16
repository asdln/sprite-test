#include "PrimitiveSetCollection.h"


void PrimitiveSetCollection::draw(GLuint uniform_mv, const QMatrix4x4& mv)
{
	QMatrix4x4 matrix_scale;
	matrix_scale.scale(scale_[0], scale_[1], scale_[2]);

	QMatrix4x4 mv_temp = mv;
	mv_temp = mv * matrix_ * matrix_front_ * matrix_scale;
	for (auto primitive_set : primitive_set_collection_)
	{
		primitive_set->draw(uniform_mv, mv_temp);
	}
}

void PrimitiveSetCollection::draw_selection(GLuint uniform_mv, const QMatrix4x4& mv)
{
	QMatrix4x4 matrix_scale;
	matrix_scale.scale(scale_[0], scale_[1], scale_[2]);

	QMatrix4x4 mv_temp = mv;
	mv_temp = mv * matrix_ * matrix_front_ * matrix_scale;
	for (auto primitive_set : primitive_set_collection_)
	{
		primitive_set->draw_selection(uniform_mv, mv_temp);
	}
}

int PrimitiveSetCollection::line_intersect(float* p0, float* p1, QMatrix4x4 ma)
{
	int result = -1;

	QMatrix4x4 matrix_scale;
	matrix_scale.scale(scale_[0], scale_[1], scale_[2]);

	QMatrix4x4 mv_temp = matrix_ * matrix_front_ * matrix_scale;

	for (auto primitive_set : primitive_set_collection_)
	{
		int res = primitive_set->line_intersect(p0, p1, mv_temp);
		if (res >= 0)
		{
			result = res;
		}
	}

	return result;
}