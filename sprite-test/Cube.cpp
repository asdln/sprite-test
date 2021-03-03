#include "Cube.h"
#include "Plane.h"

void Cube::init()
{
	calc_center();

	auto l = std::make_shared<Plane>();
	primitive_set_collection_.emplace_back(l);
	float offset = l->size() * 0.5;

	l->set_matrix(matrix_);
	l->translate(-offset, 0.0, 0.0);
	l->rotate(90, 0, 0, 1);

	auto r = std::make_shared<Plane>();
	primitive_set_collection_.emplace_back(r);

	r->set_matrix(matrix_);
	r->translate(offset, 0.0, 0.0);
	r->rotate(-90, 0, 0, 1);

	auto t = std::make_shared<Plane>();
	primitive_set_collection_.emplace_back(t);

	t->set_matrix(matrix_);
	t->translate(0.0, offset, 0.0);

	auto b = std::make_shared<Plane>();
	primitive_set_collection_.emplace_back(b);

	b->set_matrix(matrix_);
	b->translate(0.0, -offset, 0.0);
	b->rotate(180, 1, 0, 0);

	auto front = std::make_shared<Plane>();
	primitive_set_collection_.emplace_back(front);

	front->set_matrix(matrix_);
	front->translate(0, 0, offset);
	front->rotate(90.0, 1, 0, 0);

	auto back = std::make_shared<Plane>();
	primitive_set_collection_.emplace_back(back);

	back->set_matrix(matrix_);
	back->translate(0, 0, -offset);
	back->rotate(-90.0, 1, 0, 0);

	for (auto primitive_set : primitive_set_collection_)
	{
		primitive_set->init();
	}
}
