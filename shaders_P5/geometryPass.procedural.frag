#version 330 core

in vec3 vPos;
in vec4 vNormal;
in vec2 vTexCoord;
in float depth;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outPos;
layout(location = 5) out float outLit;

float rand()
{
	return fract(sin(dot(vTexCoord.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	outPos = vec4(vPos, 1.0);
	outLit = 0;
	float c = clamp(dot(vNormal, vec4(vPos, 1.0)), 0.0, 0.7);
	c += 0.5*rand();
	c -= depth * 0.013;
	c = pow(c, 10 * rand());
	outColor = vec4(c, c, c, 1.0);
}