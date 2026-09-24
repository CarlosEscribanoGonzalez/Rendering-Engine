#version 330 core

in vec4 vNormal;
in vec3 fragPos;
in vec2 texCoord;
in float depth;

out vec4 outColor;

float rand()
{
	return fract(sin(dot(texCoord.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	float c = clamp(dot(vNormal, vec4(fragPos, 1.0)), 0.0, 0.7);
	c += 0.5*rand();
	c -= depth * 0.13;
	c = pow(c, 10 * rand());
	outColor = vec4(c, c, c, 1.0);
}