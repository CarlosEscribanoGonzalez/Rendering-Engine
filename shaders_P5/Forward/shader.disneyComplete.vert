#version 330 core

in vec3 inPos;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 normal;
uniform mat4 modelView;
uniform mat4 modelViewProj;

out vec2 vtexCoord;
out vec3 vPos;
out vec3 vNormal;

void main()
{
	vtexCoord = inTexCoord;
	vNormal = normalize((normal*vec4(inNormal, 0.0)).xyz); 
	vPos = (modelView*vec4(inPos, 1.0)).xyz;
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}

