#pragma once
#include "GlobalVariables.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glut.h"

void handleFPSMovement(unsigned char key) {
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

void keyboardFunc(unsigned char key, int x, int y) {
	ImGui_ImplGLUT_KeyboardFunc(key, x, y);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard) return;
	if (key == 'p' || key == 'P') pause = !pause;
	handleFPSMovement(key);
}

int prevMouseX = 0;
int prevMouseY = 0;
void mouseFunc(int button, int state, int x, int y) {
	ImGui_ImplGLUT_MouseFunc(button, state, x, y);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) return;
	if (state == 0) {
		prevMouseX = x;
		prevMouseY = y;
	}
}

void mouseMotionFunc(int x, int y) {
	ImGui_ImplGLUT_MotionFunc(x, y);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) return;
	glm::vec3 rotationVector = glm::vec3(0.0, 0.0, 0.0);
	rotationVector += glm::vec3(0.0, 1.0, 0.0) * (float)(prevMouseX - x);
	rotationVector += glm::vec3(1.0, 0.0, 0.0) * (float)(prevMouseY - y);
	if (glm::length(rotationVector) == 0) return;
	prevMouseX = x;
	prevMouseY = y;
	sceneManager.getCurrentScene()->getCurrentCamera().rotate(rotationVector, true);
}