///////////////////////////////////////////////////////////////////////////////
// teapot.h
// ========
// vertex, normal and index array for teapot model
// (6320 polygons, 3241 vertices, 3259 normals)
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2005-10-05
// UPDATED: 2018-03-01
//
// 3D model is converted by the PolyTrans from Okino Computer Graphics, Inc.
//
// Bounding box of geometry = (-3,0,-2) to (3.434,3.15,2).
//
// drawTeapot()   : render it with VA
// drawTeapotVBO(): render it with VBO
///////////////////////////////////////////////////////////////////////////////

#ifndef TEAPOT_H
#define TEAPOT_H
#include "Geometry.h"

class Teapot : public Geometry
{
public:

	Teapot();

	~Teapot();
};


#endif
