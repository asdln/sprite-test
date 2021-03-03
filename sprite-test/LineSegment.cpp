#include "LineSegment.h"

LineSegment::LineSegment(QVector3D p1, QVector3D p2) 
{
	normal_count_ = 6;
	vertice_count_ = 6;
	indice_count_ = 2;

	vertices_ = new GLfloat[vertice_count_];
	normals_ = new GLfloat[normal_count_];
	indices_ = new GLushort[indice_count_];

	vertices_[0] = p1.x();
	vertices_[1] = p1.y();
	vertices_[2] = p1.z();

	vertices_[3] = p2.x();
	vertices_[4] = p2.y();
	vertices_[5] = p2.z();

	normals_[0] = 0;
	normals_[1] = 0;
	normals_[2] = 1;

	normals_[3] = 0;
	normals_[4] = 0;
	normals_[5] = 1;

	indices_[0] = 0;
	indices_[1] = 1;

	primitives_.emplace_back(GL_LINES, 2, GL_UNSIGNED_SHORT, 0);

	center_[0] = (p1.x() + p2.x()) * 0.5;
	center_[1] = (p1.y() + p2.y()) * 0.5;
	center_[2] = (p1.z() + p2.z()) * 0.5;
}

LineSegment::~LineSegment()
{
	delete vertices_;
	delete normals_;
	delete indices_;
}