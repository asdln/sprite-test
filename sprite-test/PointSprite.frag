#version 430 core
out vec4 fColor;
uniform sampler2D tex;

void main() {
	const vec4 color1 = vec4(0.6, 0.0, 0.0, 1.0);
    const vec4 color2 = vec4(0.9, 0.7, 1.0, 0.0);

	vec2 temp = gl_PointCoord - vec2(0.5,0.5);
    float f = dot(temp, temp);

    if (f > 0.25)
        discard;

    //fColor = vec4(1.0, 0.0, 0.0, 1.0);
	fColor = texture(tex, gl_PointCoord);
	fColor = mix(fColor, color2, smoothstep(0.1, 0.25, f));
}