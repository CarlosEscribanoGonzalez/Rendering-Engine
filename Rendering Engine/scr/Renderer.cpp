#include "Renderer.h"
#include "Scene.h"
#include "GlobalVariables.h"

void Renderer::init(RendererType t, LightingProgram* program) {
	type = t;
	if (program) lightingProgram = program;
	else lightingProgram = &shaderManager.lightingPass_default;
	finalPassProgram = &shaderManager.finalPass; 
	planeVAO = vaoManager.getVAO(meshManager.plane, shaderManager.geometryPass_default);
	initFBO(lightingFBO);
	initFBO(geometryFBO);
	for (int i = 0; i < postProcessFBOs.size(); i++) initFBO(postProcessFBOs[i]);
	resize(SCREEN_SIZE);
}

void Renderer::setLightingProgram(LightingProgram* program) {
	lightingProgram = program;
}

void Renderer::initFBO(FBO& fbo) {
	fbo.init();
	totalFBOs.push_back(fbo);
}

void Renderer::resize(unsigned int w, unsigned int h) {
	for (FBO& fbo : totalFBOs) {
		fbo.resize(w, h);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::render() {
	if (type == Forward) renderForward();
	else renderDeferred();
}

void Renderer::renderForward() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::vector<ObjectInfo*> objects = sceneManager.getCurrentScene()->getObjects();
	for (int i = 0; i < objects.size(); i++) {
		ObjectInfo& obj = *objects[i];
		if (!currentGProgram || currentGProgram->id != obj.programInfo.id) { //Avoids unnecessary GPU state changes
			currentGProgram = &obj.programInfo;
			glUseProgram(currentGProgram->id);
		}
		performForwardPass(obj);
		if (!currentVAO || currentVAO->vao != obj.vaoInfo.vao) { 
			currentVAO = &obj.vaoInfo;
			glBindVertexArray(currentVAO->vao);
		}
		glDrawElements(GL_TRIANGLES, currentVAO->mesh.nTrianglesIndex * 3, GL_UNSIGNED_INT, (void*)0);
	}
	currentGProgram = nullptr;
	currentVAO = nullptr;
	glutSwapBuffers();
}

void Renderer::renderDeferred() {
	//Geometry pass:
	glBindFramebuffer(GL_FRAMEBUFFER, geometryFBO.id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::vector<ObjectInfo*> objects = sceneManager.getCurrentScene()->getObjects();
	for (int i = 0; i < objects.size(); i++) {
		ObjectInfo& obj = *objects[i];
		if (!currentGProgram || currentGProgram->id != obj.programInfo.id) {
			currentGProgram = &obj.programInfo;
			glUseProgram(currentGProgram->id);
		}
		performGeometryPass(obj);
		if (!currentVAO || currentVAO->vao != obj.vaoInfo.vao) {
			currentVAO = &obj.vaoInfo;
			glBindVertexArray(currentVAO->vao);
		}
		glDrawElements(GL_TRIANGLES, currentVAO->mesh.nTrianglesIndex * 3, GL_UNSIGNED_INT, (void*)0);
	}
	currentGProgram = nullptr;
	currentVAO = nullptr;
	//Lighting pass:
	performLightingPass(lightingFBO.id, *lightingProgram, geometryFBO);
	//Post-procesos:
	FBO* prevFBO = &lightingFBO;
	if(enablePostProcessing) {
		for (PostProcessProgram* pp : sceneManager.getCurrentScene()->getPostProcessing()) {
			ppFBOIndex = ppFBOIndex == 0 ? 1 : 0;
			performPostProcessPass(postProcessFBOs[ppFBOIndex].id, *pp, *prevFBO);
			prevFBO = &postProcessFBOs[ppFBOIndex];
		}
	}
	if (enableBlending) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
		glBlendColor(motionBlurBrightness, motionBlurBrightness,
			motionBlurBrightness, motionBlurAlpha);
		glBlendEquation(GL_FUNC_ADD);
	}
	performLightingPass(0, *finalPassProgram, *prevFBO);
	if (enableBlending) glDisable(GL_BLEND);
	glutSwapBuffers();
}

void Renderer::performForwardPass(ObjectInfo& obj) {
	glm::mat4& model = obj.model;
	glm::mat4 view = sceneManager.getCurrentScene()->getCurrentCamera().getView();
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = sceneManager.getCurrentScene()->getCurrentCamera().getProj() * modelView;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));
	if (currentGProgram->uModelViewMat != -1)
		glUniformMatrix4fv(currentGProgram->uModelViewMat, 1, GL_FALSE, &(modelView[0][0]));
	if (currentGProgram->uModelViewProjMat != -1)
		glUniformMatrix4fv(currentGProgram->uModelViewProjMat, 1, GL_FALSE, &(modelViewProj[0][0]));
	if (currentGProgram->uNormalMat != -1)
		glUniformMatrix4fv(currentGProgram->uNormalMat, 1, GL_FALSE, &(normal[0][0]));
	if (currentGProgram->uModelMat != -1)
		glUniformMatrix4fv(currentGProgram->uModelMat, 1, GL_FALSE, &(model[0][0]));
	if (currentGProgram->uViewMat != -1)
		glUniformMatrix4fv(currentGProgram->uViewMat, 1, GL_FALSE, &(view[0][0]));
	Material& mat = obj.mat;
	if (currentGProgram->uColorTex != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mat.colorId);
		glUniform1i(currentGProgram->uColorTex, 0);
	}
	if (currentGProgram->uEmiTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, mat.emiId);
		glUniform1i(currentGProgram->uEmiTex, 1);
	}
	if (currentGProgram->uNormalTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, mat.normalId);
		glUniform1i(currentGProgram->uNormalTex, 2);
	}
	if (currentGProgram->uSpecularTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, mat.specularId);
		glUniform1i(currentGProgram->uSpecularTex, 3);
	}
	std::vector<Light> lights = sceneManager.getCurrentScene()->getLights();
	if (currentGProgram->uNumLights != -1) {
		glUniform1i(currentGProgram->uNumLights, lights.size());
	}
	if (currentGProgram->uIa != -1) {
		glUniform1f(currentGProgram->uIa, sceneManager.getCurrentScene()->getAmbientIntensity());
	}
	for (int i = 0; i < lights.size(); i++) {
		std::string base = "lights[" + std::to_string(i) + "]";
		glUniform1i(glGetUniformLocation(currentGProgram->id, (base + ".type").c_str()), lights[i].type);
		glUniform3fv(glGetUniformLocation(currentGProgram->id, (base + ".position").c_str()), 1, &lights[i].position[0]);
		glUniform3fv(glGetUniformLocation(currentGProgram->id, (base + ".diffuse").c_str()), 1, &lights[i].diffuse[0]);
		glUniform3fv(glGetUniformLocation(currentGProgram->id, (base + ".specular").c_str()), 1, &lights[i].specular[0]);
		glUniform3fv(glGetUniformLocation(currentGProgram->id, (base + ".direction").c_str()), 1, &lights[i].direction[0]);
		glUniform1f(glGetUniformLocation(currentGProgram->id, (base + ".alpha").c_str()), lights[i].alpha);
		glUniform1f(glGetUniformLocation(currentGProgram->id, (base + ".intensity").c_str()), lights[i].intensity);
		glUniform1f(glGetUniformLocation(currentGProgram->id, (base + ".angle").c_str()), lights[i].angle);
		glUniform1f(glGetUniformLocation(currentGProgram->id, (base + ".dmin").c_str()), lights[i].dmin);
		glUniform1f(glGetUniformLocation(currentGProgram->id, (base + ".d0").c_str()), lights[i].d0);
		glUniform1f(glGetUniformLocation(currentGProgram->id, (base + ".dmax").c_str()), lights[i].dmax);
	}
}

void Renderer::performGeometryPass(ObjectInfo& obj) {
	//Matrices:
	glm::mat4& model = obj.model;
	glm::mat4 view = sceneManager.getCurrentScene()->getCurrentCamera().getView();
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = sceneManager.getCurrentScene()->getCurrentCamera().getProj() * modelView;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));
	if (currentGProgram->uModelViewMat != -1)
		glUniformMatrix4fv(currentGProgram->uModelViewMat, 1, GL_FALSE, &(modelView[0][0]));
	if (currentGProgram->uModelViewProjMat != -1)
		glUniformMatrix4fv(currentGProgram->uModelViewProjMat, 1, GL_FALSE, &(modelViewProj[0][0]));
	if (currentGProgram->uNormalMat != -1)
		glUniformMatrix4fv(currentGProgram->uNormalMat, 1, GL_FALSE, &(normal[0][0]));
	if (currentGProgram->uModelMat != -1)
		glUniformMatrix4fv(currentGProgram->uModelMat, 1, GL_FALSE, &(model[0][0]));
	if (currentGProgram->uViewMat != -1)
		glUniformMatrix4fv(currentGProgram->uViewMat, 1, GL_FALSE, &(view[0][0]));
	//Textures
	Material& mat = obj.mat;
	if (currentGProgram->uColorTex != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mat.colorId);
		glUniform1i(currentGProgram->uColorTex, 0);
	}
	if (currentGProgram->uEmiTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, mat.emiId);
		glUniform1i(currentGProgram->uEmiTex, 1);
	}
	if (currentGProgram->uNormalTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, mat.normalId);
		glUniform1i(currentGProgram->uNormalTex, 2);
	}
	if (currentGProgram->uSpecularTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, mat.specularId);
		glUniform1i(currentGProgram->uSpecularTex, 3);
	}
}

void Renderer::performLightingPass(int fbo, const LightingProgram& program, const FBO& prevFBO) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (fbo != 0) glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program.id);
	if (program.uColorTex != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, prevFBO.colorBuffTexId);
		glUniform1i(program.uColorTex, 0);
	}
	if (program.uDepthTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, geometryFBO.depthBuffTexId);
		glUniform1i(program.uDepthTex, 1);
	}
	if (program.uPosTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, geometryFBO.posBuffTexId);
		glUniform1i(program.uPosTex, 2);
	}
	if (program.uNormalTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, geometryFBO.normalBuffTexId);
		glUniform1i(program.uNormalTex, 3);
	}
	if (program.uSpecularTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, prevFBO.specularBuffTexId);
		glUniform1i(program.uSpecularTex, 4);
	}
	if (program.uEmissiveTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, prevFBO.emissiveBuffTexId);
		glUniform1i(program.uEmissiveTex, 5);
	}
	if (program.uLitTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 6);
		glBindTexture(GL_TEXTURE_2D, prevFBO.litBuffTexId);
		glUniform1i(program.uLitTex, 6);
	}
	//Lights:
	std::vector<Light> lights = sceneManager.getCurrentScene()->getLights();
	if (program.uNumLights != -1) {
		glUniform1i(program.uNumLights, lights.size());
	}
	if (program.uIa != -1) {
		glUniform1f(program.uIa, sceneManager.getCurrentScene()->getAmbientIntensity());
	}
	for (int i = 0; i < lights.size(); i++) {
		std::string base = "lights[" + std::to_string(i) + "]";
		glUniform1i(glGetUniformLocation(program.id, (base + ".type").c_str()), lights[i].type);
		glUniform3fv(glGetUniformLocation(program.id, (base + ".position").c_str()), 1, &lights[i].position[0]);
		glUniform3fv(glGetUniformLocation(program.id, (base + ".diffuse").c_str()), 1, &lights[i].diffuse[0]);
		glUniform3fv(glGetUniformLocation(program.id, (base + ".specular").c_str()), 1, &lights[i].specular[0]);
		glUniform3fv(glGetUniformLocation(program.id, (base + ".direction").c_str()), 1, &lights[i].direction[0]);
		glUniform1f(glGetUniformLocation(program.id, (base + ".alpha").c_str()), lights[i].alpha);
		glUniform1f(glGetUniformLocation(program.id, (base + ".intensity").c_str()), lights[i].intensity);
		glUniform1f(glGetUniformLocation(program.id, (base + ".angle").c_str()), lights[i].angle);
		glUniform1f(glGetUniformLocation(program.id, (base + ".dmin").c_str()), lights[i].dmin);
		glUniform1f(glGetUniformLocation(program.id, (base + ".d0").c_str()), lights[i].d0);
		glUniform1f(glGetUniformLocation(program.id, (base + ".dmax").c_str()), lights[i].dmax);
	}
	if (program.uViewMat != -1)
		glUniformMatrix4fv(program.uViewMat, 1, GL_FALSE, &(sceneManager.getCurrentScene()->getCurrentCamera().getView()[0][0]));
	if (program.uNear != -1)
		glUniform1f(program.uNear, NEAR_DIST);
	if (program.uFar != -1)
		glUniform1f(program.uFar, FAR_DIST);
	glUseProgram(program.id);
	renderPlane();
}

void Renderer::performPostProcessPass(int fbo, const PostProcessProgram& program, const FBO& prevFBO) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (fbo != 0) glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program.id);
	if (program.uColorTex != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, prevFBO.colorBuffTexId);
		glUniform1i(program.uColorTex, 0);
	}
	if (program.uDepthTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, geometryFBO.depthBuffTexId);
		glUniform1i(program.uDepthTex, 1);
	}
	if (program.uPosTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, geometryFBO.posBuffTexId);
		glUniform1i(program.uPosTex, 2);
	}
	if (program.uLightPassTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, lightingFBO.colorBuffTexId);
		glUniform1i(program.uLightPassTex, 3);
	}
	if (program.uFocalDistance != -1)
		glUniform1f(program.uFocalDistance, focalDistance);
	if (program.uMaxDofDistance != -1)
		glUniform1f(program.uMaxDofDistance, maxDofDistance);
	if (program.uNear != -1)
		glUniform1f(program.uNear, NEAR_DIST);
	if (program.uFar != -1)
		glUniform1f(program.uFar, FAR_DIST);
	if (program.uMask != -1)
		glUniform1fv(program.uMask, program.mask.size, program.mask.mask);
	if (program.uTexIdx != -1)
		glUniform2fv(program.uTexIdx, program.mask.size, &program.mask.texIdx[0].x);
	if (program.uMaskSize != -1)
		glUniform1f(program.uMaskSize, program.mask.size);
	renderPlane();
}

void Renderer::renderPlane() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(planeVAO.vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::destroy() {
	for (int i = 0; i < totalFBOs.size(); i++) {
		totalFBOs[i].destroy();
	}
}