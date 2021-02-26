#pragma once

float tb_project_to_sphere(float r, float x, float y);

float getXnormalized(double x, double minx, double maxx);

float getYnormalized(double y, double miny, double maxy);

int line_triangle_intersect(float* p0, float* p1, float* v0, float* v1, float* v2);