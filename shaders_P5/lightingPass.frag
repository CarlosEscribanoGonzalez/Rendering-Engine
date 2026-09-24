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

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D posTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform sampler2D emissiveTex;
uniform sampler2D litTex;
uniform mat4 view;
uniform int numLights;
uniform Light lights[10];
uniform float Ia;

layout(location = 0) out vec4 outColor;

vec3 N;
vec3 V;
vec3 vPos;
vec3 unlitColor;
vec3 specular;

// Blinn-Phong shading
vec3 baseShade();
vec3 shade(Light l);
vec3 shadeFocal(Light l);
vec3 shadeDirectional(Light l);
float computeFdist(Light l, vec3 ligthViewPos);

void main() {
	unlitColor = texture(colorTex, texCoord).rgb;
	vec4 pos = texture(posTex, texCoord).rgba;
	vPos = pos.rgb;
	N = normalize(texture(normalTex, texCoord).rgb);
	V = normalize(-vPos);
	specular = texture(specularTex, texCoord).rgb;
	vec3 litColor = baseShade();
	for(int i = 0; i < numLights; i++){
		if(lights[i].type == 0) litColor += shade(lights[i]);
		else if(lights[i].type == 1) litColor += shadeDirectional(lights[i]);
		else litColor += shadeFocal(lights[i]);
	}
	litColor = mix(unlitColor, litColor, texture(litTex, texCoord).x);
	//pos.w es 1.0 si hay un objeto, 0.0 si no. Sin usar ifs, se evita iluminar el clearColor:
	outColor = vec4(pos.w * litColor + (1 - pos.w) * unlitColor, 1.0);
}

vec3 baseShade()
{
	return Ia * unlitColor + texture(emissiveTex, texCoord).rgb;
}

vec3 shade(Light l) {
	vec3 lPosView = (view * vec4(l.position, 1.0)).xyz; 
	vec3 L = normalize(lPosView - vPos);

	vec3 diffuse = l.diffuse * unlitColor * clamp(dot(L, N), 0.0, 1.0);

	vec3 H = normalize(L + V); 
	float specularFactor = clamp(dot(N, H), 0.0, 1.0);
	vec3 specular = l.specular * specular * pow(specularFactor, l.alpha);

	vec3 shading = (diffuse + specular) * l.intensity * computeFdist(l, lPosView);
	return clamp(shading, 0.0, 1.0);
}

vec3 shadeFocal(Light l){
	vec3 lPosView = (view * vec4(l.position, 1.0)).xyz;
	vec3 D = normalize((view * vec4(l.direction, 0.0)).xyz);
	vec3 L = normalize(lPosView - vPos);

	vec3 diffuse = l.diffuse * unlitColor * clamp(dot(L, N), 0.0, 1.0); 	

	vec3 H = normalize(V + L);
	float specularFactor = clamp(dot(H,N),0.0,1.0); 
	vec3 specular = l.specular * specular * pow(specularFactor, l.alpha); 

	float theta = radians(l.angle);
	float fdir = (dot(-L, D) - cos(theta)) / (1 - cos(theta));
	fdir = pow(max(fdir, 0), 10);

	vec3 shading = (diffuse + specular) * l.intensity * computeFdist(l, lPosView) * fdir;
	return clamp(shading, 0.0, 1.0);
}

vec3 shadeDirectional(Light l)
{
	vec3 L = normalize(-(view * vec4(l.direction, 0.0)).xyz);
	vec3 H = normalize(V + L);

	vec3 diffuse = l.diffuse * unlitColor * clamp(dot(L, N), 0.0, 1.0); 
	
	float specularFactor = clamp(dot(H, N),0.0,1.0); 
	vec3 specular = l.specular * specular * pow(specularFactor, l.alpha); 

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