#version 330 core

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D depthTex;
uniform float near;
uniform float far;
uniform float focalDistance;
uniform float maxDistanceFactor;
uniform float[100] mask;
uniform vec2[100] texIdx;
uniform float maskSize;

layout(location = 0) out vec4 outColor;

void main()
{
	vec2 ts = vec2(1.0) / vec2 (textureSize (colorTex,0));
	float depth = -(near * far) / (far + texture(depthTex, texCoord).r * (near - far));
	float dof = abs(depth - focalDistance) * maxDistanceFactor;
	dof = clamp (dof, 0.0, 1.0);
	dof *= dof; 
	vec4 color = vec4 (0.0);
	for (uint i = 0u; i < maskSize; i++)
	{
		vec2 iidx = texCoord + ts * texIdx[i] * dof;
		color += texture(colorTex, iidx,0.0) * mask[i];
	}
	outColor = color;
} 