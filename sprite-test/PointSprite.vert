#version 430 core

layout(location = 0) in vec4 vPos;
layout(location = 1) in vec3 vVertNormal;
//layout(location = 0) out vec4 vColor;
uniform highp mat4 matrix_mv;
uniform highp mat4 matrix_p;

out vec3 Normal;
out vec4 Pos;

void main()
{
	Pos = matrix_mv * vPos;
	gl_Position = matrix_p * Pos;
	mat3 matNoral =  transpose(inverse(mat3(matrix_mv)));
	Normal = normalize(matNoral * vVertNormal);
	
	//gl_PointSize = 60.0;
	//gl_PointSize = 64.0;
	//gl_PointSize = (pos.z/pos.w * -1.0 + 0.930501938) / (0.930501938-0.835168540) * 64.0 + 1.0;
	//gl_PointSize = (pos.z - 1.2) / 1.6 * 64.0 + 1.0;
	//gl_Position = pos;
	//vColor = vec4(1.0, 0.0, 0.0, 1.0);
}
