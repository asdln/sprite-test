#version 430 core
layout(location = 0) in vec4 vPos;
uniform highp mat4 matrix_mv;

uniform highp mat4 matrix_p;

void main() {
   vec4 pos = matrix_p * matrix_mv * vPos;
   gl_Position = pos;
   gl_PointSize = 40;
}