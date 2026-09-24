#pragma once
#include "masks.h"
#include <GL/glew.h>
#include <vector>

struct GeometryProgram {
	unsigned int id;
	unsigned int vShader;
	unsigned int fShader;
	//Uniform variables
	unsigned int uModelViewMat;
	unsigned int uModelViewProjMat;
	unsigned int uNormalMat;
	unsigned int uViewMat;
	unsigned int uModelMat;
	//Uniform textures
	unsigned int uColorTex;
	unsigned int uEmiTex;
	unsigned int uNormalTex;
	unsigned int uSpecularTex;
	//Attributes
	unsigned int inPos;
	unsigned int inColor;
	unsigned int inNormal;
	unsigned int inTexCoord;
	unsigned int inTangent;
	//For forward shaders:
	unsigned int uLights;
	unsigned int uNumLights;
	unsigned int uIa;

	bool operator<(const GeometryProgram& other) const {
		return id < other.id;
	}
};

struct LightingProgram {
	unsigned int id;
	unsigned int vShader;
	unsigned int fShader;
	unsigned int uColorTex;
	unsigned int uDepthTex;
	unsigned int uPosTex;
	unsigned int uNormalTex;
	unsigned int uSpecularTex;
	unsigned int uEmissiveTex;
	unsigned int uLitTex;
	unsigned int inPos;
	unsigned int uLights;
	unsigned int uNumLights;
	unsigned int uIa;
	unsigned int uViewMat;
	unsigned int uNear;
	unsigned int uFar;
};

struct Mask {
	float* mask = gauss3x3;
	glm::vec2* texIdx = texIdx3x3;
	int size = 9;
};

struct PostProcessProgram {
	unsigned int id;
	unsigned int vShader;
	unsigned int fShader;
	unsigned int uColorTex;
	unsigned int uDepthTex;
	unsigned int uPosTex;
	unsigned int uLightPassTex;
	unsigned int uNear;
	unsigned int uFar;
	unsigned int uFocalDistance;
	unsigned int uMaxDofDistance;
	unsigned int uMask;
	unsigned int uTexIdx;
	unsigned int uMaskSize;
	Mask mask = {};
};

class ShaderManager {
private:
	std::vector<GeometryProgram*> geometryPrograms;
	std::vector<LightingProgram*> lightingPrograms;
	std::vector<PostProcessProgram*> ppPrograms;

	GeometryProgram& initGeometryProgram(const char* vname, const char* fname);
	LightingProgram& initLightingProgram(const char* vname, const char* fname);
	PostProcessProgram& initPostProcessProgram(const char* vname, const char* fname, Mask mask);
	GLuint loadShader(const char* fileName, GLenum type);

public:
	GeometryProgram forward_default;
	GeometryProgram forward_bump;
	GeometryProgram forward_discard;
	GeometryProgram forward_disney;
	GeometryProgram forward_fog;
	GeometryProgram forward_procedural;

	GeometryProgram geometryPass_default;
	GeometryProgram geometryPass_bump;
	GeometryProgram geometryPass_discard;
	GeometryProgram geometryPass_procedural;

	LightingProgram lightingPass_default;
	LightingProgram lightingPass_fog;
	LightingProgram lightingPass_disney;
	LightingProgram finalPass;

	PostProcessProgram pp_edgeDetection;
	PostProcessProgram pp_sobelX;
	PostProcessProgram pp_sobelY;
	PostProcessProgram pp_bloom;
	PostProcessProgram pp_dof;

	void initShaders();
	void destroyShaders();
};