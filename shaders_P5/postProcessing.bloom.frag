#version 330 core

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D depthTex;
uniform float[100] mask;
uniform vec2[100] texIdx;
uniform float maskSize;

float threshold = 0.8;
float bloomIntensity = 10;

layout(location = 0) out vec4 outColor;

void main()
{
	vec2 ts = vec2(1.0) / vec2 (textureSize (colorTex,0)); 
	vec4 color = vec4 (0.0);
	for (uint i = 0u; i < maskSize; i++)
	{
		vec2 iidx = texCoord + ts * texIdx[i];
		vec4 c = texture(colorTex, iidx,0.0);
		float luminance = step(threshold, c.r - threshold/4) * c.r;
		luminance += step(threshold, c.g) * c.g;
		luminance += step(threshold, c.b - threshold/2) * c.b;
		color += c * luminance * mask[i];
	}
	outColor = clamp(texture(colorTex, texCoord) + color * bloomIntensity, 0.0, 1.0);
} 