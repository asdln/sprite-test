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
