#version 330 core
in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outPos; 
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outSpecular;
layout(location = 4) out vec3 outEmissive;
layout(location = 5) out float outLit;

void main() {
	float conditionValue = pow(vTexCoord.x - 0.5, 2) + pow(vTexCoord.y - 0.5, 2);
	if(conditionValue >= 0.2) discard;

	outPos = vec4(vPos, 1.0);
	outNormal = normalize(vNormal);
	outSpecular = texture(specularTex, vTexCoord).rgb;
	outEmissive = texture(emiTex, vTexCoord).rgb;
	outColor = vec4(texture(colorTex, vTexCoord).rgb, 1.0);
	outLit = 1;
}