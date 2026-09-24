#version 330 core

in vec3 inPos;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 vPos;
out vec4 vNormal;
out vec2 vTexCoord;
out float depth;

float maxVibration = 0.1;

void main() {
	vPos = (modelView * vec4(inPos, 1.0)).xyz;
	vNormal = normalize((normal * vec4(inNormal, 0.0)));
	vTexCoord = inTexCoord;
	float offset = - maxVibration * 0.5 + maxVibration*fract(sin(dot(vPos.xy ,vec2(12.9898,78.233))));
	gl_Position = modelViewProj * vec4(inPos + offset, 1.0);
	depth = gl_Position.z;
}
