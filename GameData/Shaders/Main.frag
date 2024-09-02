#version 330 core

in vec3 fColor;

out vec4 oColor;

void main()
{
	float depth = (1- gl_FragCoord.z) * 30;
	depth =  clamp(depth, 0.05, 0.95);

	oColor = vec4(fColor * depth, 1.0);
}