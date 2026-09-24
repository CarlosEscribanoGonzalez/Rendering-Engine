#version 330 core

in vec2 texCoord;

uniform sampler2D colorTex;

out vec4 outColor;

void main()
{
	outColor = texture(colorTex, texCoord);
} 