#version 330 core

struct Light {
	int type;
	vec3 position;
	vec3 diffuse;
	vec3 specular;	
	vec3 direction;
	float alpha;
	float intensity;
	float angle;
	float dmin;
	float d0;
	float dmax;
};

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;

uniform sampler2D colorTex;
uniform sampler2D specularTex;
uniform sampler2D emiTex;
uniform sampler2D normalTex;
uniform mat4 view;
uniform int numLights;
uniform Light lights[10];
uniform float Ia;

// Per object properties (Material BRDF definition)
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;

out vec4 outColor;

vec3 N;

// Blinn-Phong shading
vec3 baseShade();
vec3 shade(Light l);
vec3 shadeFocal(Light l);
vec3 shadeDirectional(Light l);
float computeFdist(Light l, vec3 lightViewPos);

void main() {
	N = normalize(vNormal);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);
	vec3 normalTex = texture(normalTex, vTexCoord).rgb * 2.0 - 1.0;
	N = normalize(TBN * normalTex);

	Ka = texture(colorTex, vTexCoord).rgb;
	Kd = texture(colorTex, vTexCoord).rgb;
	Ks = texture(specularTex, vTexCoord).rgb;
	Ke = texture(emiTex, vTexCoord).rgb;
	vec3 color = baseShade();
	for(int i = 0; i < numLights; i++){
		if(lights[i].type == 0) color += shade(lights[i]);
		else if(lights[i].type == 1) color += shadeDirectional(lights[i]);
		else color += shadeFocal(lights[i]);
	}
	outColor = vec4(color, 1.0);
}

vec3 baseShade()
{
	return Ia * Ka + Ke;
}

vec3 shade(Light l) {
	vec3 lPosView = (view * vec4(l.position, 1.0)).xyz; 
	vec3 L = normalize(lPosView - vPos);
	vec3 V = normalize(-vPos);

	vec3 diffuse = l.diffuse * Kd * clamp(dot(L, N), 0.0, 1.0);

	vec3 H = normalize(L + V); 
	float specularFactor = clamp(dot(N, H), 0.0, 1.0);
	vec3 specular = l.specular * Ks * pow(specularFactor, l.alpha);

	vec3 shading = (diffuse + specular) * l.intensity * computeFdist(l, lPosView);
	return clamp(shading, 0.0, 1.0);
}

vec3 shadeFocal(Light l){
	vec3 lPosView = (view * vec4(l.position, 1.0)).xyz;
	vec3 D = normalize((view * vec4(l.direction, 0.0)).xyz);
	vec3 L = normalize(lPosView - vPos);
	vec3 V = normalize(-vPos); 

	vec3 diffuse = l.diffuse * Kd * clamp(dot(L, N), 0.0, 1.0); 	

	vec3 H = normalize(V + L);
	float specularFactor = clamp(dot(H,N),0.0,1.0); 
	vec3 specular = l.specular * Ks * pow(specularFactor, l.alpha); 

	float theta = radians(l.angle);
	float fdir = (dot(-L, D) - cos(theta)) / (1 - cos(theta));
	fdir = pow(max(fdir, 0), 10);

	vec3 shading = (diffuse + specular) * l.intensity * computeFdist(l, lPosView) * fdir;
	return clamp(shading, 0.0, 1.0);
}

vec3 shadeDirectional(Light l)
{
	vec3 L = normalize(-(view * vec4(l.direction, 0.0)).xyz);
	vec3 V = normalize(-vPos); 
	vec3 H = normalize(V + L);

	vec3 diffuse = l.diffuse * Kd * clamp(dot(L, N), 0.0, 1.0); 
	
	float specularFactor = clamp(dot(H, N),0.0,1.0); 
	vec3 specular = l.specular * Ks * pow(specularFactor, l.alpha); 

	vec3 shading = (diffuse + specular) * l.intensity;
	return clamp(shading, 0.0, 1.0);
}

float computeFdist(Light l, vec3 lightViewPos){
	float dist = length(lightViewPos - vPos);
	float fdist = l.d0/max(dist, l.dmin);
	fdist *= fdist;
	float fwin = 1 - pow(dist/l.dmax, 4);
	fwin = max(fwin, 0);
	fwin *= fwin;
	return fdist * fwin;
}