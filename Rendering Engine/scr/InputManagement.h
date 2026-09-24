#pragma once
#include "GlobalVariables.h"

std::vector<const char*> movementTypes = { "Camera", "Lights", "PostProcessing", "Scenes"};
int movementIdx = 0;

void editCamera(unsigned char key) {
	if (key == 'c' || key == 'C') sceneManager.getCurrentScene()->changeCamera();
	glm::vec3 moveDirection = glm::vec3(0.0);
	if (key == 'w' || key == 'W') moveDirection.z += 1;
	else if (key == 's' || key == 'S') moveDirection.z -= 1;
	else if (key == 'a' || key == 'A') moveDirection.x -= 1;
	else if (key == 'd' || key == 'D') moveDirection.x += 1;
	else if (key == 'e' || key == 'E') moveDirection.y += 1;
	else if (key == 'q' || key == 'Q') moveDirection.y -= 1;
	if (glm::length(moveDirection) != 0) sceneManager.getCurrentScene()->getCurrentCamera().translate(moveDirection);
	glm::vec3 rotationDir = glm::vec3(0.0, 0.0, 0.0);
	if (key == '6') rotationDir.y -= 1;
	else if (key == '4') rotationDir.y += 1;
	else if (key == '8') rotationDir.x += 1;
	else if (key == '2') rotationDir.x -= 1;
	if (glm::length(rotationDir) != 0) sceneManager.getCurrentScene()->getCurrentCamera().rotate(rotationDir);
}

void editLighting(unsigned char key) {
	glm::vec3 movement = glm::vec3(0.0);
	float intensityVariation = 0;
	if (key == 'w' || key == 'W') movement.z -= 1;
	else if (key == 's' || key == 'S') movement.z += 1;
	else if (key == 'a' || key == 'A') movement.x -= 1;
	else if (key == 'd' || key == 'D') movement.x += 1;
	else if (key == 'q' || key == 'Q') movement.y -= 1;
	else if (key == 'e' || key == 'E') movement.y += 1;
	else if (key == '+') intensityVariation += 1;
	else if (key == '-') intensityVariation -= 1;
	for (Light& l : sceneManager.getCurrentScene()->getLights())
		l.update(movement, intensityVariation);
	if (key == '1') renderer.setLightingProgram(&shaderManager.lightingPass_default);
	else if (key == '2') renderer.setLightingProgram(&shaderManager.lightingPass_fog);
	else if (key == '3') renderer.setLightingProgram(&shaderManager.lightingPass_disney);
	else if (key == 'f' || key == 'F') renderer.setType(Forward);
	else if (key == 'd' || key == 'D') renderer.setType(Deferred);
}

void editPP(unsigned char key) {
	if (key == 'e' || key == 'E') enablePostProcessing = !enablePostProcessing;
	if (key == 'm' || key == 'M') enableBlending = !enableBlending;
	else if (key == 'a' || key == 'A') motionBlurBrightness -= motionBlurVariationSpeed;
	else if (key == 'd' || key == 'D') motionBlurBrightness += motionBlurVariationSpeed;
	else if (key == 'w' || key == 'W') motionBlurAlpha += motionBlurVariationSpeed;
	else if (key == 's' || key == 'S') motionBlurAlpha -= motionBlurVariationSpeed;
	motionBlurBrightness = glm::clamp(motionBlurBrightness, (GLclampf)0, (GLclampf)1);
	motionBlurAlpha = glm::clamp(motionBlurAlpha, (GLclampf)0, (GLclampf)0.95);

	if (key == '8') focalDistance -= dofVariationSpeed;
	else if (key == '2') focalDistance += dofVariationSpeed;
	else if (key == '+') maxDofDistance += dofVariationSpeed / 2;
	else if (key == '-') maxDofDistance -= dofVariationSpeed / 2;
	maxDofDistance = std::max(maxDofDistance, 0.0f);
	focalDistance = std::min(focalDistance, 0.0f);
	if (key == '3') shaderManager.pp_dof.mask = { gauss3x3, texIdx3x3, size3x3 };
	else if (key == '5') shaderManager.pp_dof.mask = { gauss5x5, texIdx5x5, size5x5 };
	else if (key == '7') shaderManager.pp_dof.mask = { gauss7x7, texIdx7x7, size7x7 };
	else if (key == '9') shaderManager.pp_dof.mask = { gauss9x9, texIdx9x9, size9x9 };
}

void changeScene(unsigned char key) {
	if (key == '1') sceneManager.changeScene(sceneManager.deferredScene_1);
	else if (key == '2') sceneManager.changeScene(sceneManager.deferredScene_2);
	else if (key == '3') sceneManager.changeScene(sceneManager.deferredScene_3);
	else if (key == '4') sceneManager.changeScene(sceneManager.deferredScene_4);
	else if (key == '5' || key == 'f' || key == 'F') 
		sceneManager.changeScene(sceneManager.forwardScene);
}

void keyboardFunc(unsigned char key, int x, int y) {
	const char* movement = movementTypes[movementIdx];
	if (key == ' ') {
		if (++movementIdx == movementTypes.size()) movementIdx = 0;
		std::cout << "Movement type: " << movementTypes[movementIdx] << std::endl;
	}
	else if (key == 'p' || key == 'P') pause = !pause;
	else if (movement == "Camera") editCamera(key);
	else if (movement == "Lights") editLighting(key);
	else if (movement == "PostProcessing") editPP(key);
	else changeScene(key);
}

int prevMouseX = 0;
int prevMouseY = 0;
void mouseFunc(int button, int state, int x, int y) {
	if (state == 0) {
		prevMouseX = x;
		prevMouseY = y;
	}
}

void mouseMotionFunc(int x, int y) {
	glm::vec3 rotationVector = glm::vec3(0.0, 0.0, 0.0);
	rotationVector += glm::vec3(0.0, 1.0, 0.0) * (float)(prevMouseX - x);
	rotationVector += glm::vec3(1.0, 0.0, 0.0) * (float)(prevMouseY - y);
	if (glm::length(rotationVector) == 0) return;
	prevMouseX = x;
	prevMouseY = y;
	sceneManager.getCurrentScene()->getCurrentCamera().rotate(rotationVector, true);
}