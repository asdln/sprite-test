#version 430 core

layout(location = 0) in vec4 vPos;
layout(location = 1) in vec3 vVertNormal;

uniform vec4 vVertColor;
uniform highp mat4 matrix_mv;
uniform highp mat4 matrix_p;

out vec3 Normal;
out vec4 Color;

void main()
{
	gl_Position = matrix_p * matrix_mv * vPos;
	mat3 matNoral =  transpose(inverse(mat3(matrix_mv)));
	Normal = normalize(matNoral * vVertNormal);
	Color = vVertColor;
}
