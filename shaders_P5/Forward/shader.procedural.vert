#version 330 core

in vec3 inPos;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 modelViewProj;
uniform mat4 model;
uniform mat4 normal;

out vec4 vNormal;
out vec3 fragPos;
out vec2 texCoord;
out float depth;

void main()
{
	texCoord = inTexCoord;
	fragPos = (model * vec4(inPos, 1.0)).xyz;
	vNormal = normal * vec4(inNormal, 1.0);
	float offset = - 0.02 + 0.04*fract(sin(dot(fragPos.xy ,vec2(12.9898,78.233))));
	gl_Position = modelViewProj * vec4(inPos + offset, 1.0);
	depth = gl_Position.z;
}