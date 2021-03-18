#include "Sprite.h"

Sprite::Sprite(GLfloat* vertices, int vertice_count)
{
	primitives_.emplace_back(Primitive(GL_POINTS, vertice_count / 3, GL_UNSIGNED_SHORT, 0));

	vertice_count_ = vertice_count;
	vertices_ = new GLfloat[vertice_count_];
	memcpy(vertices_, vertices, vertice_count * sizeof(GLfloat));

	indice_count_ = vertice_count_ / 3;
	indices_ = new GLushort[indice_count_];

	for (int i = 0; i < indice_count_; i++)
	{
		indices_[i] = i;
	}
}

Sprite::~Sprite()
{
	delete vertices_;
	delete indices_;
}