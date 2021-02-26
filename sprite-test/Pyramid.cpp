#include "Pyramid.h"

Pyramid::Pyramid()
{
	
}

void Pyramid::init()
{
	l.set_matrix(matrix_);
	l.translate(-0.5, 0.0, 0.0);
	l.rotate(-45, 0, 0, 1);
	l.rotate(-90, 0, 1, 0);

	r.set_matrix(matrix_);
	r.translate(0.5, 0.0, 0.0);
	r.rotate(45, 0, 0, 1);
	r.rotate(90, 0, 1, 0);

	f.set_matrix(matrix_);
	f.translate(0, 0, 0.5);
	f.rotate(-45, 1, 0, 0);

	b.set_matrix(matrix_);
	b.translate(0, 0, -0.5);
	b.rotate(45, 1, 0, 0);
	b.rotate(180, 0, 1, 0);

	p.set_matrix(matrix_);
	p.rotate(180, 1, 0, 0);

	l.init();
	r.init();

	f.init();
	b.init();

	p.init();
}

void Pyramid::draw()
{
	l.draw();
	r.draw();
	f.draw();
	b.draw();

	p.draw();
}