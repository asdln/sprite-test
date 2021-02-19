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

#include <QtGui/QOpenGLFunctions_4_3_Core>
#include "teapot_data.h"

class Teapot : protected QOpenGLFunctions_4_3_Core
{
protected:

	GLuint vert;
	GLuint vbo_vertex;
	GLuint vbo_normal;

	GLuint ebo;

public:

	void Init()
	{
		initializeOpenGLFunctions();

		glGenVertexArrays(1, &vert);
		glBindVertexArray(vert);

		glGenBuffers(1, &vbo_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(teapotVertices), teapotVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 /*3*/, (void*)0);
		glEnableVertexAttribArray(0);

 		glGenBuffers(1, &vbo_normal);
 		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
 		glBufferData(GL_ARRAY_BUFFER, sizeof(teapotNormals), teapotNormals, GL_STATIC_DRAW);
 		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
 		glEnableVertexAttribArray(1);

		glGenBuffers(1, &ebo);                           // create a vbo
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);                       // activate vbo id to use
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapotIndices), teapotIndices, GL_STATIC_DRAW);    // upload data to video card

		glBindVertexArray(0);
	}

	///////////////////////////////////////////////////////////////////////////////
	// draw teapot using only offset instead of absolute pointers.
	// The caller must bind buffer ids and set the starting offset before call this
	// functions. (glBindBufferARB, glVertexPointer, glNormalPointer, glIndexPointer)
	///////////////////////////////////////////////////////////////////////////////
	void drawTeapotVBO()
	{
		float shininess = 15.0f;
		float diffuseColor[4] = { 0.929524f, 0.796542f, 0.178823f, 1.0f };
		float specularColor[4] = { 1.00000f, 0.980392f, 0.549020f, 1.0f };

		// set color using glMaterial (gold-yellow)
		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // range 0 ~ 128
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuseColor);


		// set ambient and diffuse color using glColorMaterial (gold-yellow)
		//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		//glColor3fv(diffuseColor);

		glBindVertexArray(vert);

		// start to render polygons
// 		glEnableClientState(GL_NORMAL_ARRAY);
// 		glEnableClientState(GL_VERTEX_ARRAY);

		// use only offset here instead of absolute pointer addresses
		glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_SHORT, (GLushort*)0 + 0);
		glDrawElements(GL_TRIANGLE_STRIP, 78, GL_UNSIGNED_SHORT, (GLushort*)0 + 12);
		glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, (GLushort*)0 + 90);
		glDrawElements(GL_TRIANGLE_STRIP, 70, GL_UNSIGNED_SHORT, (GLushort*)0 + 125);
		glDrawElements(GL_TRIANGLE_STRIP, 65, GL_UNSIGNED_SHORT, (GLushort*)0 + 195);
		glDrawElements(GL_TRIANGLE_STRIP, 37, GL_UNSIGNED_SHORT, (GLushort*)0 + 260);
		glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, (GLushort*)0 + 297);
		glDrawElements(GL_TRIANGLE_STRIP, 32, GL_UNSIGNED_SHORT, (GLushort*)0 + 332);
		glDrawElements(GL_TRIANGLE_STRIP, 56, GL_UNSIGNED_SHORT, (GLushort*)0 + 364);
		glDrawElements(GL_TRIANGLE_STRIP, 45, GL_UNSIGNED_SHORT, (GLushort*)0 + 420);
		glDrawElements(GL_TRIANGLE_STRIP, 41, GL_UNSIGNED_SHORT, (GLushort*)0 + 465);
		glDrawElements(GL_TRIANGLE_STRIP, 37, GL_UNSIGNED_SHORT, (GLushort*)0 + 506);
		glDrawElements(GL_TRIANGLE_STRIP, 33, GL_UNSIGNED_SHORT, (GLushort*)0 + 543);
		glDrawElements(GL_TRIANGLE_STRIP, 29, GL_UNSIGNED_SHORT, (GLushort*)0 + 576);
		glDrawElements(GL_TRIANGLE_STRIP, 25, GL_UNSIGNED_SHORT, (GLushort*)0 + 605);
		glDrawElements(GL_TRIANGLE_STRIP, 21, GL_UNSIGNED_SHORT, (GLushort*)0 + 630);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, (GLushort*)0 + 651);
		glDrawElements(GL_TRIANGLE_STRIP, 13, GL_UNSIGNED_SHORT, (GLushort*)0 + 668);
		glDrawElements(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_SHORT, (GLushort*)0 + 681);
		glDrawElements(GL_TRIANGLE_STRIP, 27, GL_UNSIGNED_SHORT, (GLushort*)0 + 690);
		glDrawElements(GL_TRIANGLE_STRIP, 16, GL_UNSIGNED_SHORT, (GLushort*)0 + 717);
		glDrawElements(GL_TRIANGLE_STRIP, 22, GL_UNSIGNED_SHORT, (GLushort*)0 + 733);
		glDrawElements(GL_TRIANGLE_STRIP, 50, GL_UNSIGNED_SHORT, (GLushort*)0 + 755);
		glDrawElements(GL_TRIANGLE_STRIP, 42, GL_UNSIGNED_SHORT, (GLushort*)0 + 805);
		glDrawElements(GL_TRIANGLE_STRIP, 43, GL_UNSIGNED_SHORT, (GLushort*)0 + 847);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLushort*)0 + 890);
		glDrawElements(GL_TRIANGLE_STRIP, 143, GL_UNSIGNED_SHORT, (GLushort*)0 + 894);
		glDrawElements(GL_TRIANGLE_STRIP, 234, GL_UNSIGNED_SHORT, (GLushort*)0 + 1037);
		glDrawElements(GL_TRIANGLE_STRIP, 224, GL_UNSIGNED_SHORT, (GLushort*)0 + 1271);
		glDrawElements(GL_TRIANGLE_STRIP, 71, GL_UNSIGNED_SHORT, (GLushort*)0 + 1495);
		glDrawElements(GL_TRIANGLE_STRIP, 69, GL_UNSIGNED_SHORT, (GLushort*)0 + 1566);
		glDrawElements(GL_TRIANGLE_STRIP, 67, GL_UNSIGNED_SHORT, (GLushort*)0 + 1635);
		glDrawElements(GL_TRIANGLE_STRIP, 65, GL_UNSIGNED_SHORT, (GLushort*)0 + 1702);
		glDrawElements(GL_TRIANGLE_STRIP, 63, GL_UNSIGNED_SHORT, (GLushort*)0 + 1767);
		glDrawElements(GL_TRIANGLE_STRIP, 61, GL_UNSIGNED_SHORT, (GLushort*)0 + 1830);
		glDrawElements(GL_TRIANGLE_STRIP, 59, GL_UNSIGNED_SHORT, (GLushort*)0 + 1891);
		glDrawElements(GL_TRIANGLE_STRIP, 57, GL_UNSIGNED_SHORT, (GLushort*)0 + 1950);
		glDrawElements(GL_TRIANGLE_STRIP, 55, GL_UNSIGNED_SHORT, (GLushort*)0 + 2007);
		glDrawElements(GL_TRIANGLE_STRIP, 53, GL_UNSIGNED_SHORT, (GLushort*)0 + 2062);
		glDrawElements(GL_TRIANGLE_STRIP, 51, GL_UNSIGNED_SHORT, (GLushort*)0 + 2115);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 2166);
		glDrawElements(GL_TRIANGLE_STRIP, 50, GL_UNSIGNED_SHORT, (GLushort*)0 + 2169);
		glDrawElements(GL_TRIANGLE_STRIP, 48, GL_UNSIGNED_SHORT, (GLushort*)0 + 2219);
		glDrawElements(GL_TRIANGLE_STRIP, 46, GL_UNSIGNED_SHORT, (GLushort*)0 + 2267);
		glDrawElements(GL_TRIANGLE_STRIP, 44, GL_UNSIGNED_SHORT, (GLushort*)0 + 2313);
		glDrawElements(GL_TRIANGLE_STRIP, 42, GL_UNSIGNED_SHORT, (GLushort*)0 + 2357);
		glDrawElements(GL_TRIANGLE_STRIP, 40, GL_UNSIGNED_SHORT, (GLushort*)0 + 2399);
		glDrawElements(GL_TRIANGLE_STRIP, 38, GL_UNSIGNED_SHORT, (GLushort*)0 + 2439);
		glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_SHORT, (GLushort*)0 + 2477);
		glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, (GLushort*)0 + 2513);
		glDrawElements(GL_TRIANGLE_STRIP, 32, GL_UNSIGNED_SHORT, (GLushort*)0 + 2547);
		glDrawElements(GL_TRIANGLE_STRIP, 30, GL_UNSIGNED_SHORT, (GLushort*)0 + 2579);
		glDrawElements(GL_TRIANGLE_STRIP, 28, GL_UNSIGNED_SHORT, (GLushort*)0 + 2609);
		glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_SHORT, (GLushort*)0 + 2637);
		glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_SHORT, (GLushort*)0 + 2663);
		glDrawElements(GL_TRIANGLE_STRIP, 22, GL_UNSIGNED_SHORT, (GLushort*)0 + 2687);
		glDrawElements(GL_TRIANGLE_STRIP, 20, GL_UNSIGNED_SHORT, (GLushort*)0 + 2709);
		glDrawElements(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_SHORT, (GLushort*)0 + 2729);
		glDrawElements(GL_TRIANGLE_STRIP, 16, GL_UNSIGNED_SHORT, (GLushort*)0 + 2747);
		glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_SHORT, (GLushort*)0 + 2763);
		glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_SHORT, (GLushort*)0 + 2777);
		glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_SHORT, (GLushort*)0 + 2789);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (GLushort*)0 + 2799);
		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, (GLushort*)0 + 2807);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 2813);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 2816);
		glDrawElements(GL_TRIANGLE_STRIP, 200, GL_UNSIGNED_SHORT, (GLushort*)0 + 2819);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3019);
		glDrawElements(GL_TRIANGLE_STRIP, 66, GL_UNSIGNED_SHORT, (GLushort*)0 + 3022);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3088);
		glDrawElements(GL_TRIANGLE_STRIP, 209, GL_UNSIGNED_SHORT, (GLushort*)0 + 3091);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3300);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3303);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3306);
		glDrawElements(GL_TRIANGLE_STRIP, 38, GL_UNSIGNED_SHORT, (GLushort*)0 + 3309);
		glDrawElements(GL_TRIANGLE_STRIP, 15, GL_UNSIGNED_SHORT, (GLushort*)0 + 3347);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3362);
		glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_SHORT, (GLushort*)0 + 3365);
		glDrawElements(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_SHORT, (GLushort*)0 + 3391);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3400);
		glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_SHORT, (GLushort*)0 + 3403);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3417);
		glDrawElements(GL_TRIANGLE_STRIP, 115, GL_UNSIGNED_SHORT, (GLushort*)0 + 3420);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3535);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3538);
		glDrawElements(GL_TRIANGLE_STRIP, 39, GL_UNSIGNED_SHORT, (GLushort*)0 + 3541);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3580);
		glDrawElements(GL_TRIANGLE_STRIP, 91, GL_UNSIGNED_SHORT, (GLushort*)0 + 3583);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3674);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3677);
		glDrawElements(GL_TRIANGLE_STRIP, 31, GL_UNSIGNED_SHORT, (GLushort*)0 + 3680);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3711);
		glDrawElements(GL_TRIANGLE_STRIP, 67, GL_UNSIGNED_SHORT, (GLushort*)0 + 3714);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3781);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3784);
		glDrawElements(GL_TRIANGLE_STRIP, 23, GL_UNSIGNED_SHORT, (GLushort*)0 + 3787);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3810);
		glDrawElements(GL_TRIANGLE_STRIP, 45, GL_UNSIGNED_SHORT, (GLushort*)0 + 3813);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3858);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3861);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3864);
		glDrawElements(GL_TRIANGLE_STRIP, 32, GL_UNSIGNED_SHORT, (GLushort*)0 + 3867);
		glDrawElements(GL_TRIANGLE_STRIP, 38, GL_UNSIGNED_SHORT, (GLushort*)0 + 3899);
		glDrawElements(GL_TRIANGLE_STRIP, 15, GL_UNSIGNED_SHORT, (GLushort*)0 + 3937);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3952);
		glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_SHORT, (GLushort*)0 + 3955);
		glDrawElements(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_SHORT, (GLushort*)0 + 3981);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 3990);
		glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_SHORT, (GLushort*)0 + 3993);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 4007);
		glDrawElements(GL_TRIANGLE_STRIP, 135, GL_UNSIGNED_SHORT, (GLushort*)0 + 4010);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 4145);
		glDrawElements(GL_TRIANGLE_STRIP, 76, GL_UNSIGNED_SHORT, (GLushort*)0 + 4148);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 4224);
		glDrawElements(GL_TRIANGLE_STRIP, 60, GL_UNSIGNED_SHORT, (GLushort*)0 + 4227);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 4287);
		glDrawElements(GL_TRIANGLE_STRIP, 23, GL_UNSIGNED_SHORT, (GLushort*)0 + 4290);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 4313);
		glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_SHORT, (GLushort*)0 + 4316);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 4342);
		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, (GLushort*)0 + 4345);
		glDrawElements(GL_TRIANGLE_STRIP, 947, GL_UNSIGNED_SHORT, (GLushort*)0 + 4351);
		glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, (GLushort*)0 + 5298);
		glDrawElements(GL_TRIANGLE_STRIP, 31, GL_UNSIGNED_SHORT, (GLushort*)0 + 5333);
		glDrawElements(GL_TRIANGLE_STRIP, 27, GL_UNSIGNED_SHORT, (GLushort*)0 + 5364);
		glDrawElements(GL_TRIANGLE_STRIP, 23, GL_UNSIGNED_SHORT, (GLushort*)0 + 5391);
		glDrawElements(GL_TRIANGLE_STRIP, 20, GL_UNSIGNED_SHORT, (GLushort*)0 + 5414);
		glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_SHORT, (GLushort*)0 + 5434);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5458);
		glDrawElements(GL_TRIANGLE_STRIP, 28, GL_UNSIGNED_SHORT, (GLushort*)0 + 5461);
		glDrawElements(GL_TRIANGLE_STRIP, 32, GL_UNSIGNED_SHORT, (GLushort*)0 + 5489);
		glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_SHORT, (GLushort*)0 + 5521);
		glDrawElements(GL_TRIANGLE_STRIP, 76, GL_UNSIGNED_SHORT, (GLushort*)0 + 5557);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5633);
		glDrawElements(GL_TRIANGLE_STRIP, 67, GL_UNSIGNED_SHORT, (GLushort*)0 + 5636);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5703);
		glDrawElements(GL_TRIANGLE_STRIP, 59, GL_UNSIGNED_SHORT, (GLushort*)0 + 5706);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5765);
		glDrawElements(GL_TRIANGLE_STRIP, 51, GL_UNSIGNED_SHORT, (GLushort*)0 + 5768);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5819);
		glDrawElements(GL_TRIANGLE_STRIP, 43, GL_UNSIGNED_SHORT, (GLushort*)0 + 5822);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5865);
		glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, (GLushort*)0 + 5868);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5903);
		glDrawElements(GL_TRIANGLE_STRIP, 27, GL_UNSIGNED_SHORT, (GLushort*)0 + 5906);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5933);
		glDrawElements(GL_TRIANGLE_STRIP, 19, GL_UNSIGNED_SHORT, (GLushort*)0 + 5936);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5955);
		glDrawElements(GL_TRIANGLE_STRIP, 11, GL_UNSIGNED_SHORT, (GLushort*)0 + 5958);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 5969);
		glDrawElements(GL_TRIANGLE_STRIP, 30, GL_UNSIGNED_SHORT, (GLushort*)0 + 5972);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 6002);
		glDrawElements(GL_TRIANGLE_STRIP, 11, GL_UNSIGNED_SHORT, (GLushort*)0 + 6005);
		glDrawElements(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_SHORT, (GLushort*)0 + 6016);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 6034);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLushort*)0 + 6037);
		glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_SHORT, (GLushort*)0 + 6040);
		glDrawElements(GL_TRIANGLE_STRIP, 122, GL_UNSIGNED_SHORT, (GLushort*)0 + 6045);
		glDrawElements(GL_TRIANGLE_STRIP, 75, GL_UNSIGNED_SHORT, (GLushort*)0 + 6167);
		glDrawElements(GL_TRIANGLE_STRIP, 71, GL_UNSIGNED_SHORT, (GLushort*)0 + 6242);
		glDrawElements(GL_TRIANGLE_STRIP, 67, GL_UNSIGNED_SHORT, (GLushort*)0 + 6313);
		glDrawElements(GL_TRIANGLE_STRIP, 63, GL_UNSIGNED_SHORT, (GLushort*)0 + 6380);
		glDrawElements(GL_TRIANGLE_STRIP, 59, GL_UNSIGNED_SHORT, (GLushort*)0 + 6443);
		glDrawElements(GL_TRIANGLE_STRIP, 55, GL_UNSIGNED_SHORT, (GLushort*)0 + 6502);
		glDrawElements(GL_TRIANGLE_STRIP, 51, GL_UNSIGNED_SHORT, (GLushort*)0 + 6557);
		glDrawElements(GL_TRIANGLE_STRIP, 47, GL_UNSIGNED_SHORT, (GLushort*)0 + 6608);
		glDrawElements(GL_TRIANGLE_STRIP, 43, GL_UNSIGNED_SHORT, (GLushort*)0 + 6655);
		glDrawElements(GL_TRIANGLE_STRIP, 39, GL_UNSIGNED_SHORT, (GLushort*)0 + 6698);
		glDrawElements(GL_TRIANGLE_STRIP, 35, GL_UNSIGNED_SHORT, (GLushort*)0 + 6737);
		glDrawElements(GL_TRIANGLE_STRIP, 31, GL_UNSIGNED_SHORT, (GLushort*)0 + 6772);
		glDrawElements(GL_TRIANGLE_STRIP, 27, GL_UNSIGNED_SHORT, (GLushort*)0 + 6803);
		glDrawElements(GL_TRIANGLE_STRIP, 23, GL_UNSIGNED_SHORT, (GLushort*)0 + 6830);
		glDrawElements(GL_TRIANGLE_STRIP, 19, GL_UNSIGNED_SHORT, (GLushort*)0 + 6853);
		glDrawElements(GL_TRIANGLE_STRIP, 15, GL_UNSIGNED_SHORT, (GLushort*)0 + 6872);
		glDrawElements(GL_TRIANGLE_STRIP, 11, GL_UNSIGNED_SHORT, (GLushort*)0 + 6887);
		glDrawElements(GL_TRIANGLE_STRIP, 7, GL_UNSIGNED_SHORT, (GLushort*)0 + 6898);

// 		glDisableClientState(GL_VERTEX_ARRAY);	// disable vertex arrays
// 		glDisableClientState(GL_NORMAL_ARRAY);	// disable normal arrays
	}

};


#endif
