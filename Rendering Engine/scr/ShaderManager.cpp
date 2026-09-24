#include "ShaderManager.h"
#include "auxiliar.h"
#include <iostream>

void ShaderManager::initShaders() {
	forward_default = initGeometryProgram("../shaders_P5/Forward/multipleLights.vert",
		"../shaders_P5/Forward/multipleLights.frag");
	forward_bump = initGeometryProgram("../shaders_P5/Forward/shader.bump.vert",
		"../shaders_P5/Forward/shader.bump.frag");
	forward_discard = initGeometryProgram("../shaders_P5/Forward/shader.Discard.vert",
		"../shaders_P5/Forward/shader.Discard.frag");
	forward_disney = initGeometryProgram("../shaders_P5/Forward/shader.disneyComplete.vert",
		"../shaders_P5/Forward/shader.disneyComplete.frag");
	forward_fog = initGeometryProgram("../shaders_P5/Forward/shader.fog.vert",
		"../shaders_P5/Forward/shader.fog.frag");
	forward_procedural = initGeometryProgram("../shaders_P5/Forward/shader.procedural.vert",
		"../shaders_P5/Forward/shader.procedural.frag");

	geometryPass_default = initGeometryProgram("../shaders_P5/geometryPass.vert",
		"../shaders_P5/geometryPass.frag");
	geometryPass_bump = initGeometryProgram("../shaders_P5/geometryPass.bump.vert",
		"../shaders_P5/geometryPass.bump.frag");
	geometryPass_discard = initGeometryProgram("../shaders_P5/geometryPass.discard.vert",
		"../shaders_P5/geometryPass.discard.frag");
	geometryPass_procedural = initGeometryProgram("../shaders_P5/geometryPass.procedural.vert",
		"../shaders_P5/geometryPass.procedural.frag");

	lightingPass_default = initLightingProgram("../shaders_P5/lightingPass.vert",
		"../shaders_P5/lightingPass.frag");
	lightingPass_fog = initLightingProgram("../shaders_P5/lightingPass.fog.vert",
		"../shaders_P5/lightingPass.fog.frag");
	lightingPass_disney = initLightingProgram("../shaders_P5/lightingPass.disney.vert",
		"../shaders_P5/lightingPass.disney.frag");
	finalPass = initLightingProgram("../shaders_P5/finalPass.vert",
		"../shaders_P5/finalPass.frag");

	pp_edgeDetection = initPostProcessProgram("../shaders_P5/postProcessing.generic.vert",
		"../shaders_P5/postProcessing.generic.frag", { edgeDetection, texIdx3x3, size3x3 });
	pp_sobelX = initPostProcessProgram("../shaders_P5/postProcessing.sobel.vert",
		"../shaders_P5/postProcessing.sobel.frag", { sobelX });
	pp_sobelY = initPostProcessProgram("../shaders_P5/postProcessing.sobel.vert",
		"../shaders_P5/postProcessing.sobel.frag", { sobelY });
	pp_bloom = initPostProcessProgram("../shaders_P5/postProcessing.bloom.vert",
		"../shaders_P5/postProcessing.bloom.frag", { gauss9x9, texIdx9x9, size9x9 });
	pp_dof = initPostProcessProgram("../shaders_P5/postProcessing.dof.vert",
		"../shaders_P5/postProcessing.dof.frag", { gauss3x3, texIdx3x3, size3x3 });
}

void ShaderManager::destroyShaders() {
	for (GeometryProgram* program : geometryPrograms) {
		glDetachShader(program->id, program->vShader);
		glDetachShader(program->id, program->fShader);
		glDeleteShader(program->vShader);
		glDeleteShader(program->fShader);
		glDeleteProgram(program->id);
	}
	for (LightingProgram* program : lightingPrograms) {
		glDetachShader(program->id, program->vShader);
		glDetachShader(program->id, program->fShader);
		glDeleteShader(program->vShader);
		glDeleteShader(program->fShader);
		glDeleteProgram(program->id);
	}
	for (PostProcessProgram* program : ppPrograms) {
		glDetachShader(program->id, program->vShader);
		glDetachShader(program->id, program->fShader);
		glDeleteShader(program->vShader);
		glDeleteShader(program->fShader);
		glDeleteProgram(program->id);
	}
}

GeometryProgram& ShaderManager::initGeometryProgram(const char* vname, const char* fname) {
	unsigned int vshader = loadShader(vname, GL_VERTEX_SHADER);
	unsigned int fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	unsigned int program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");
	glBindAttribLocation(program, 4, "inTangent");

	glLinkProgram(program);
	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteProgram(program);
		program = 0;
		exit(-1);
	}

	GeometryProgram* info = new GeometryProgram();
	info->id = program;
	info->vShader = vshader;
	info->fShader = fshader;
	//Matrices:
	info->uNormalMat = glGetUniformLocation(program, "normal");
	info->uModelViewMat = glGetUniformLocation(program, "modelView");
	info->uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
	info->uViewMat = glGetUniformLocation(program, "view");
	info->uModelMat = glGetUniformLocation(program, "model");
	//Textures:
	info->uColorTex = glGetUniformLocation(program, "colorTex");
	info->uEmiTex = glGetUniformLocation(program, "emiTex");
	info->uNormalTex = glGetUniformLocation(program, "normalTex");
	info->uSpecularTex = glGetUniformLocation(program, "specularTex");
	//Attributes:
	info->inPos = glGetAttribLocation(program, "inPos");
	info->inColor = glGetAttribLocation(program, "inColor");
	info->inNormal = glGetAttribLocation(program, "inNormal");
	info->inTexCoord = glGetAttribLocation(program, "inTexCoord");
	info->inTangent = glGetAttribLocation(program, "inTangent");
	//For forward shaders:
	info->uLights = glGetUniformLocation(program, "lights");
	info->uNumLights = glGetUniformLocation(program, "numLights");
	info->uIa = glGetUniformLocation(program, "Ia");
	geometryPrograms.push_back(info);
	return *info;
}

LightingProgram& ShaderManager::initLightingProgram(const char* vname, const char* fname) {
	unsigned int vshader = loadShader(vname, GL_VERTEX_SHADER);
	unsigned int fshader = loadShader(fname, GL_FRAGMENT_SHADER);
	unsigned int program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glBindAttribLocation(program, 0, "inPos");
	glLinkProgram(program);
	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;
		glDeleteProgram(program);
		program = 0;
		exit(-1);
	}
	LightingProgram info = {};
	info.id = program;
	info.vShader = vshader;
	info.fShader = fshader;
	info.uColorTex = glGetUniformLocation(program, "colorTex");
	info.uDepthTex = glGetUniformLocation(program, "depthTex");
	info.uPosTex = glGetUniformLocation(program, "posTex");
	info.uNormalTex = glGetUniformLocation(program, "normalTex");
	info.uSpecularTex = glGetUniformLocation(program, "specularTex");
	info.uEmissiveTex = glGetUniformLocation(program, "emissiveTex");
	info.uLitTex = glGetUniformLocation(program, "litTex");
	info.uLights = glGetUniformLocation(program, "lights");
	info.uNumLights = glGetUniformLocation(program, "numLights");
	info.uIa = glGetUniformLocation(program, "Ia");
	info.uViewMat = glGetUniformLocation(program, "view");
	info.inPos = glGetAttribLocation(program, "inPos");
	info.uNear = glGetUniformLocation(program, "near");
	info.uFar = glGetUniformLocation(program, "far");
	return info;
}

PostProcessProgram& ShaderManager::initPostProcessProgram(const char* vname, const char* fname, Mask mask) {
	unsigned int vshader = loadShader(vname, GL_VERTEX_SHADER);
	unsigned int fshader = loadShader(fname, GL_FRAGMENT_SHADER);
	unsigned int program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glBindAttribLocation(program, 0, "inPos");
	glLinkProgram(program);
	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;
		glDeleteProgram(program);
		program = 0;
		exit(-1);
	}
	PostProcessProgram* info = new PostProcessProgram();
	info->id = program;
	info->vShader = vshader;
	info->fShader = fshader;
	info->uColorTex = glGetUniformLocation(program, "colorTex");
	info->uDepthTex = glGetUniformLocation(program, "depthTex");
	info->uPosTex = glGetUniformLocation(program, "posTex");
	info->uLightPassTex = glGetUniformLocation(program, "lightPassTex");
	info->uNear = glGetUniformLocation(program, "near");
	info->uFar = glGetUniformLocation(program, "far");
	info->uFocalDistance = glGetUniformLocation(program, "focalDistance");
	info->uMaxDofDistance = glGetUniformLocation(program, "maxDistanceFactor");
	info->uMask = glGetUniformLocation(program, "mask");
	info->uTexIdx = glGetUniformLocation(program, "texIdx");
	info->uMaskSize = glGetUniformLocation(program, "maskSize");
	info->mask = mask;
	ppPrograms.push_back(info);
	return *info;
}

GLuint ShaderManager::loadShader(const char* fileName, GLenum type) {
	unsigned int fileLen;
	char* source = loadStringFromFile(fileName, fileLen);
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&source, (const GLint*)&fileLen);
	glCompileShader(shader);
	delete[] source;
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader);
		exit(-1);
	}
	return shader;
}