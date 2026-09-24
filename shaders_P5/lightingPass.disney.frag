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

const float PI = 3.141592653589793;
const float EPSILON = 1e-5;
const int MATERIAL_COUNT = 4;

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

// Per object properties (Material BRDF definition)
struct DisneyMaterial {
	vec3 baseColor;			// Albedo
	float subsurface; 		// Subsurface scattering blend
	float metallic; 		// 0 = Dielectric, 1 = Metal
	float specular; 		// Specular amount (dielectric specular scale)
	float specularTint; 	// Tint specular towards base color
	float roughness; 		// Surface roughness
	float sheen; 			// Velvet-like sheen
	float sheenTint; 		// Tint for sheen
	float clearcoat;  		// Clearcoat layer
	float clearcoatGloss; 	// Clearcoat glossiness
};

layout(location = 0) out vec4 outColor;

// D: GTR2 (GGX / Trowbridge-Reitz) normal distribution function
float D_GGX(float NdotH, float alpha) {
	float a2 = alpha * alpha;
	float denom = (NdotH * NdotH) * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;
	return a2 / max(denom, EPSILON);
}

// F: Schlick Fresnel approximation
vec3 F_Schlick(vec3 F0, float cosTheta) {
    // Schlick: F = F0 + (1 - F0) * (1 - cosTheta)^5
	float pow5 = pow(1.0 - cosTheta, 5.0);
	return F0 + (1.0 - F0) * pow5;
}

// Schlick-GGX geometry helper for one direction
float G_Schlick_GGX(float NdotV, float k) {
	return NdotV / (NdotV * (1.0 - k) + k);
}

// G: Smith geometry term using Schlick-GGX
float G_Smith(float NdotV, float NdotL, float k) {
	return G_Schlick_GGX(NdotV, k) * G_Schlick_GGX(NdotL, k);
}

// Schlick scalar for sheen
float F_SchlickScalar(float F0, float cosTheta) {
	return F0 * pow((1 - cosTheta), 5);
}

// GTR1 used by Disney for clearcoat (normalized)
float D_GTR1(float NdotH, float alpha) {
	float alpha2 = alpha * alpha;
	float D = alpha2 - 1;
	D /= (PI * log(alpha2) * (1 + (alpha2 - 1)*NdotH*NdotH));
	return D;
}

// Main Disney BRDF evaluation
vec3 disneyBRDF(DisneyMaterial m, vec3 N, vec3 V, vec3 L) {
	vec3 brdfColor = vec3(0.0);

	vec3 H = normalize(L + V);
	float NdotL = clamp(dot(N, L), 0.0, 1.0); // cos(theta_l)
	float NdotV = clamp(dot(N, V), 0.0, 1.0); // cos(theta_v)
	float NdotH = clamp(dot(N, H), 0.0, 1.0); // cos(theta_h) ; H es la micronormal, normal de la microfaceta
	float VdotH = clamp(dot(V, H), 0.0, 1.0); // cos(theta_d)

    // Specular term (GGX)
	// Map roughness to alpha
	float alpha = max(m.roughness * m.roughness, EPSILON); //No queremos que sea 0

	// Compute D term (Microfacets normal distribution) 
	float D = D_GGX(NdotH, alpha);

	// Use specularTint
	vec3 tint = normalize(m.baseColor);
	vec3 specularColor = mix(vec3(1.0), tint, m.specularTint);
	vec3 dielectricF0 = vec3(0.04) * m.specular * specularColor;

	// Compute F term (Fresnel)
	vec3 F0 = mix(dielectricF0, m.baseColor, m.metallic);
	vec3 F = F_Schlick(F0, VdotH);

	// Compute G term (Visibility)
	float k = m.roughness + 1.0;
	k = (k * k) / 8;
	float G = G_Smith(NdotV, NdotL, k);

	vec3 specular = D * F * G / max((4.0 * NdotL * NdotV), EPSILON);
	brdfColor += specular;

    // Diffuse term
	float F_D90 = 0.5 + 2.0 * m.roughness * pow(VdotH, 2.0);
	float FL = pow(1.0 - NdotL, 5);
	float FV = pow(1.0 - NdotV, 5);
	float fdL = 1.0 + (F_D90 - 1.0) * FL;
	float fdV = 1.0 + (F_D90 - 1.0) * FV;
	vec3 diffuse = m.baseColor/PI * (fdL * fdV) * (1.0 - m.metallic);// Metals do not have a diffuse (Lambertian) component
	brdfColor += diffuse;

    // Sheen term
	float sheenFactor = F_SchlickScalar(m.sheen, VdotH);
	vec3 sheenColor = mix(vec3(1.0), m.baseColor, m.sheenTint);
	vec3 sheen = sheenColor * sheenFactor;
	brdfColor += sheen * (1 - m.metallic);

    // Clearcoat term
	if(m.clearcoat > 0.0) {
		float alpha = mix(0.1, 0.001, m.clearcoatGloss);
		float D = D_GTR1(NdotH, alpha);
		vec3 F = F_Schlick(vec3(0.04), VdotH);
		vec3 clearcoat = m.clearcoat * 0.25 * D * F;
		brdfColor += clearcoat;
	}

    // Subsurface term (approx — slight retro-reflection mix)
	if(m.subsurface > 0.0) {
		float mult = 10; //Multiplicador para que se note más
		vec3 ss = F_SchlickScalar(m.subsurface, NdotL) * m.baseColor / PI;
		brdfColor += mult * ss;
	}
	
	return brdfColor * NdotL;	
}

vec3 N;
vec3 V;
vec3 vPos;
vec3 unlitColor;

// Blinn-Phong shading
vec3 baseShade(DisneyMaterial m);
vec3 shade(DisneyMaterial m, Light l);
vec3 shadeFocal(DisneyMaterial m, Light l);
vec3 shadeDirectional(DisneyMaterial m, Light l);
float computeFdist(Light l, vec3 ligthViewPos);

void main() {
	int selectedMaterial = 2;
	const DisneyMaterial materials[MATERIAL_COUNT] = DisneyMaterial[](
		//Sin nada
		DisneyMaterial(vec3(0.0, 0.0, 0.0), 0.0, 0.0, 1.0, 0.0, 0.4, 0.0, 0.0, 0.0, 0.0),
		//Sheen
		DisneyMaterial(vec3(0.35, 0.02, 0.20), 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.2, 0.0, 0.0),
		//Clearcoat
		DisneyMaterial(vec3(0.8, 0.2, 0.1), 0.0, 0.0, 1.0, 0.0, 0.4, 0.0, 0.0, 1.0, 1.0),
		//Subsurface
		DisneyMaterial(vec3(0.8, 0.2, 0.1), 1.0, 0.0, 1.0, 0.0, 0.4, 0.0, 0.0, 0.0, 0.0));
	DisneyMaterial m = materials[selectedMaterial];
	
	unlitColor = texture(colorTex, texCoord).rgb;
	vec4 pos = texture(posTex, texCoord).rgba;
	vPos = pos.rgb;
	N = normalize(texture(normalTex, texCoord).rgb);
	V = normalize(-vPos);
	vec3 litColor = baseShade(m);
	for(int i = 0; i < numLights; i++){
		if(lights[i].type == 0)	litColor += shade(m, lights[i]);
		else if(lights[i].type == 1) litColor += shadeDirectional(m, lights[i]);
		else litColor += shadeFocal(m, lights[i]);
	}
	litColor = mix(unlitColor, litColor, texture(litTex, texCoord).x);
	//pos.w es 1.0 si hay un objeto, 0.0 si no. Sin usar ifs, se evita iluminar el clearColor:
 	outColor = vec4(pos.w * litColor + (1 - pos.w) * unlitColor, 1.0);
}

vec3 baseShade(DisneyMaterial m)
{
	return Ia * m.baseColor * (1.0 - m.metallic) + texture(emissiveTex, texCoord).rgb;
}

vec3 shade(DisneyMaterial m, Light l) {
	vec3 lPosView = (view * vec4(l.position, 1.0)).xyz; 
	vec3 L = normalize(lPosView - vPos);
	vec3 brdf = disneyBRDF(m, N, V, L);
	vec3 shading = l.intensity * computeFdist(l, lPosView) * l.diffuse * brdf;
	return clamp(shading, 0.0, 1.0);
}

vec3 shadeFocal(DisneyMaterial m, Light l){
	vec3 lPosView = (view * vec4(l.position, 1.0)).xyz;
	vec3 D = normalize((view * vec4(l.direction, 0.0)).xyz);
	vec3 L = normalize(lPosView - vPos);
	vec3 brdf = disneyBRDF(m, N, V, L);
	float theta = radians(l.angle);
	float fdir = (dot(-L, D) - cos(theta)) / (1 - cos(theta));
	fdir = pow(max(fdir, 0), 10);
	vec3 shading = l.intensity * computeFdist(l, lPosView) * fdir * l.diffuse * brdf;
	return clamp(shading, 0.0, 1.0);
}

vec3 shadeDirectional(DisneyMaterial m, Light l)
{
	vec3 L = normalize(-(view * vec4(l.direction, 0.0)).xyz);
	vec3 brdf = disneyBRDF(m, N, V, L);
	vec3 shading = l.intensity * l.diffuse * brdf;
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