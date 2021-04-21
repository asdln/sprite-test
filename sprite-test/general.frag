#version 430 core

out vec4 fColor;

uniform vec4 Ambient;

uniform vec3 LightColor;
uniform vec3 LightDirection;

uniform vec3 HalfVector;
uniform float Shininess;
uniform float Strength;

in vec3 Normal;
in vec4 Color;

void main()
{
	//fColor = texture(tex, gl_PointCoord);
    //fColor = mix(fColor, color2, smoothstep(0.1, 0.25, f));
	vec4 color = Color;//vec4(0.0, 0.0, 1.0, 1.0);
	
	float diffuse = max(0.0f, dot(Normal, -LightDirection));
    float specular = max(0.0f, dot(Normal, HalfVector));
    if(diffuse <= 0.00001)
        specular = 0.0f;
    else
        specular = pow(specular, Shininess);
    vec4 ReflectLight = vec4(LightColor*specular*Strength, 0.0);

    vec4 fColor1 = min(color*Ambient+ReflectLight, vec4(1.0));
	fColor = min(vec4(diffuse, diffuse, diffuse, 1.0), vec4(1.0));
	fColor = max(Ambient, fColor);
}