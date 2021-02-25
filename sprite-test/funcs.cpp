#include "funcs.h"
#include <QtMath>

float tb_project_to_sphere(float r, float x, float y)
{
	float d, t, z;

	d = qSqrt(x*x + y * y);
	/* Inside sphere */
	if (d < r * 0.70710678118654752440)
	{
		z = qSqrt(r*r - d * d);
	}                            /* On hyperbola */
	else
	{
		t = r / 1.41421356237309504880;
		z = t * t / d;
	}
	return z;
}

float getXnormalized(double x, double minx, double maxx)
{
	return 2.0f*(x - minx) / (maxx - minx) - 1.0f;
}

float getYnormalized(double y, double miny, double maxy)
{
	return -(2.0f*(y - miny) / (maxy - miny) - 1.0f);
}

void plucker(float* a, float* b, float* out)
{
	out[0] = a[0] * b[1] - b[0] * a[1];
	out[1] = a[0] * b[2] - b[0] * a[2];
	out[2] = a[0] - b[0];
	out[3] = a[1] * b[2] - b[1] * a[2];
	out[4] = a[2] - b[2];
	out[5] = b[1] - a[1];
}

float sideOp(float* a, float*b)
{
	float res = a[0] * b[4] + a[1] * b[5] + a[2] * b[3] + a[3] * b[2] + a[4] * b[0] + a[5] * b[1];
	return res;
}

int linesegment_triangle_intersect(float* p0, float* p1, float* v0, float* v1, float* v2)
{
	float e1[6];
	float e2[6];
	float e3[6];
	float L[6];

	plucker(v1, v0, e1);
	plucker(v2, v1, e2);
	plucker(v0, v2, e3);
	plucker(p0, p1, L);

	float s1 = sideOp(L, e1);
	float s2 = sideOp(L, e2);
	float s3 = sideOp(L, e3);

	int res = -1;

	if (s1 == 0 && s2 == 0 && s3 == 0)
	{
		//print("线和三角形共面");
		res = 0;
	}
	else if ((s1 > 0 && s2 > 0 && s3 > 0) || (s1 < 0 && s2 < 0 && s3 < 0))
	{
		//print("线穿过三角形")
		res = 1;
	}
	else if ((s1 == 0 && s2*s3 > 0) || (s2 == 0 && s1*s3 > 0) || (s3 == 0 && s1*s2 > 0))
	{
		//print("线穿过三角形边缘")
		res = 2;
	}
	else if ((s1 == 0 && (s2 == 0)) || (s1 == 0 && (s3 == 0)) || (s2 == 0 && (s3 == 0)))
	{
		//print("线穿过三角形顶点")
		res = 3;
	}

	return res;
}
