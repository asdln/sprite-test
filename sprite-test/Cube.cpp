#include "Cube.h"

void Cube::init()
{
	float offset = l.size() * 0.5;

	l.set_matrix(matrix_);
	l.translate(-offset, 0.0, 0.0);
	l.rotate(90, 0, 0, 1);

	r.set_matrix(matrix_);
	r.translate(offset, 0.0, 0.0);
	r.rotate(-90, 0, 0, 1);

	t.set_matrix(matrix_);
	t.translate(0.0, offset, 0.0);

	b.set_matrix(matrix_);
	b.translate(0.0, -offset, 0.0);
	b.rotate(180, 1, 0, 0);

	front.set_matrix(matrix_);
	front.translate(0, 0, offset);
	front.rotate(90.0, 1, 0, 0);

	back.set_matrix(matrix_);
	back.translate(0, 0, -offset);
	back.rotate(-90.0, 1, 0, 0);

	l.init();
	r.init();

	t.init();
	b.init();

	front.init();
	back.init();
}

void Cube::draw()
{
	l.draw();
	r.draw();

	t.draw();
	b.draw();

	front.draw();
	back.draw();
}