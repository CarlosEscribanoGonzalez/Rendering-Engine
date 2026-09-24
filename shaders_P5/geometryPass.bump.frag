#version 330 core

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outPos; 
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outSpecular;
layout(location = 4) out vec3 outEmissive;
layout(location = 5) out float outLit;

void main() {
	//Bump:
	vec3 N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);
	vec3 normalTex = texture(normalTex, vTexCoord).rgb * 2.0 - 1.0;
	outNormal = normalize(TBN * normalTex);
	outPos = vec4(vPos, 1.0);
	outSpecular = texture(specularTex, vTexCoord).rgb;
	outEmissive = texture(emiTex, vTexCoord).rgb;
	outColor = vec4(texture(colorTex, vTexCoord).rgb, 1.0);
	outLit = 1;
}