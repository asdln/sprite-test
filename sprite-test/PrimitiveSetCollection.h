#pragma once

#include "PrimitiveSet.h"
class PrimitiveSetCollection : public Geometry
{
public:

	virtual void draw()
	{
		for (auto primitive_set : primitive_set_collection_)
		{
			primitive_set->draw();
		}
	}

	virtual void draw_selection()
	{
		for (auto primitive_set : primitive_set_collection_)
		{
			primitive_set->draw_selection();
		}
	}

	int line_intersect(float* p0, float* p1)
	{
		for (auto primitive_set : primitive_set_collection_)
		{
			primitive_set->line_intersect(p0, p1);
		}

		return -1;
	}

protected:

	std::vector<std::shared_ptr<PrimitiveSet>> primitive_set_collection_;

};

