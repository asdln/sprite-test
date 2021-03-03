#include "Pyramid.h"
#include "Triangle.h"
#include "Plane.h"

Pyramid::Pyramid()
{
	
}

void Pyramid::init()
{
	center_[0] = 0.0f;
	center_[1] = 0.25f;
	center_[2] = 0.0f;

	calc_center();

	auto l = std::make_shared<Triangle>();
	auto r = std::make_shared<Triangle>();
	auto f = std::make_shared<Triangle>();
	auto b = std::make_shared<Triangle>();

	auto p = std::make_shared<Plane>();

	primitive_set_collection_.emplace_back(l);
	primitive_set_collection_.emplace_back(r);
	primitive_set_collection_.emplace_back(f);
	primitive_set_collection_.emplace_back(b);
	primitive_set_collection_.emplace_back(p);

	l->set_matrix(matrix_);
	l->translate(-0.5, 0.0, 0.0);
	l->rotate(-45, 0, 0, 1);
	l->rotate(-90, 0, 1, 0);

	r->set_matrix(matrix_);
	r->translate(0.5, 0.0, 0.0);
	r->rotate(45, 0, 0, 1);
	r->rotate(90, 0, 1, 0);

	f->set_matrix(matrix_);
	f->translate(0, 0, 0.5);
	f->rotate(-45, 1, 0, 0);

	b->set_matrix(matrix_);
	b->translate(0, 0, -0.5);
	b->rotate(45, 1, 0, 0);
	b->rotate(180, 0, 1, 0);

	p->set_matrix(matrix_);
	p->rotate(180, 1, 0, 0);

	l->init();
	r->init();

	f->init();
	b->init();

 	p->init();
}
