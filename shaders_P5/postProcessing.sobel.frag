#version 330 core

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D posTex;
uniform sampler2D lightPassTex;
uniform float[100] mask;
uniform vec2[100] texIdx;
uniform float maskSize;

layout(location = 0) out vec4 outColor;

void main()
{
	vec2 ts = vec2(1.0) / vec2 (textureSize(lightPassTex,0));
	vec4 sobelColor = vec4 (0.0);
	for (uint i = 0u; i < maskSize; i++)
	{
		vec2 iidx = texCoord + ts * texIdx[i];
		sobelColor += texture(lightPassTex, iidx, 0.0) * mask[i];
	}
	float mixValue = texture(posTex, texCoord).w * 0.5; //El fondo no se toca
	outColor = mix(texture(colorTex, texCoord), sobelColor, mixValue);
} 