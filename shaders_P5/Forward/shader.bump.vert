#version 330 core

in vec3 inPos;
in vec3 inNormal;
in vec2 inTexCoord;
in vec3 inTangent;

uniform mat4 normal;
uniform mat4 modelView;
uniform mat4 modelViewProj;

out vec3 vPos;
out vec3 vNormal;
out vec3 vTangent;
out vec2 vTexCoord;

void main()
{
	vPos = (modelView * vec4(inPos, 1.0)).xyz;
	vNormal = normalize((normal * vec4(inNormal, 0.0)).xyz);
	vTexCoord = inTexCoord;
	vTangent = normalize((normal*vec4(inTangent, 0.0)).xyz);
	gl_Position = modelViewProj * vec4(inPos, 1.0);
}

