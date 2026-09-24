#pragma once
#include "ShaderManager.h"
#include "VAOManager.h"
#include "FBO.h"

struct ObjectInfo;

enum RendererType {
	Forward,
	Deferred
};

class Renderer {
private:
	RendererType type;
	//Shaders:
	GeometryProgram* currentGProgram;
	LightingProgram* lightingProgram;
	LightingProgram* finalPassProgram;
	//FBOs and output:
	FBO geometryFBO;
	FBO lightingFBO;
	std::vector<FBO> postProcessFBOs = std::vector<FBO>(2);
	std::vector<FBO> totalFBOs;
	int ppFBOIndex = 0;
	//VAOs
	VAOInfo* currentVAO;
	VAOInfo planeVAO;

	void initFBO(FBO& fbo);
	void renderForward();
	void renderDeferred();
	void performForwardPass(ObjectInfo& obj);
	void performGeometryPass(ObjectInfo& obj);
	void performLightingPass(int fbo, const LightingProgram& program, const FBO& prevFBO);
	void performPostProcessPass(int fbo, const PostProcessProgram& program, const FBO& prevFBO);
	void renderPlane();

public:
	void init(RendererType t, LightingProgram* program = nullptr);
	void setType(RendererType t) { type = t; }
	void setLightingProgram(LightingProgram* program);
	void resize(unsigned int w, unsigned int h);
	void render();
	void destroy();
};