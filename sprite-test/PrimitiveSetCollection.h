#pragma once

#include "PrimitiveSet.h"
class PrimitiveSetCollection : public Geometry
{
public:

	virtual void draw(GLuint uniform_mv, const QMatrix4x4& mv) override;

	virtual void draw_selection(GLuint uniform_mv, const QMatrix4x4& mv) override;

	int line_intersect(float* p0, float* p1, QMatrix4x4 ma) override;

protected:

	std::vector<std::shared_ptr<PrimitiveSet>> primitive_set_collection_;

};

