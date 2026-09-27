#pragma once
#include "GlobalVariables.h"
#include "Light.h"
#include "Scene.h"
#include "SceneManager.h"
#include "IMGUI/imgui.h"

void drawScenePanel() {
	ImGui::Begin("Scene");

	if (renderer.getType() == Deferred) {
		ImGui::Separator();
		ImGui::Text("Selected scene:");
		if (ImGui::RadioButton("1", sceneManager.getCurrentScene() == sceneManager.deferredScene_1))
			sceneManager.changeScene(sceneManager.deferredScene_1);
		ImGui::SameLine();
		if (ImGui::RadioButton("2", sceneManager.getCurrentScene() == sceneManager.deferredScene_2))
			sceneManager.changeScene(sceneManager.deferredScene_2);
		ImGui::SameLine();
		if (ImGui::RadioButton("3", sceneManager.getCurrentScene() == sceneManager.deferredScene_3))
			sceneManager.changeScene(sceneManager.deferredScene_3);
		ImGui::SameLine();
		if (ImGui::RadioButton("4", sceneManager.getCurrentScene() == sceneManager.deferredScene_4))
			sceneManager.changeScene(sceneManager.deferredScene_4);
	}

	ImGui::Separator();
	if (ImGui::Button("Change camera")) {
		sceneManager.getCurrentScene()->changeCamera();
	}

	ImGui::End();
}

void drawLightingPanel() {
	ImGui::Begin("Lighting");

	if (renderer.getType() == Deferred) {
		ImGui::Separator();
		ImGui::Text("Lighting pass:");
		if (ImGui::RadioButton("Standard", renderer.getLightingProgram() == &shaderManager.lightingPass_default))
			renderer.setLightingProgram(&shaderManager.lightingPass_default);
		if (ImGui::RadioButton("Fog", renderer.getLightingProgram() == &shaderManager.lightingPass_fog))
			renderer.setLightingProgram(&shaderManager.lightingPass_fog);
		if (ImGui::RadioButton("Disney 2012", renderer.getLightingProgram() == &shaderManager.lightingPass_disney))
			renderer.setLightingProgram(&shaderManager.lightingPass_disney);
	}
	
	ImGui::Separator();
	ImGui::Text("Light intensity mult:");
	ImGui::SliderFloat("value", &lightIntensityMult, 0.0f, 10.0f);

	ImGui::End();
}

void drawPostProcessingPanel() {
	ImGui::Begin("Post-processing");
	ImGui::Checkbox("Enable Post-processing", &enablePostProcessing);

	ImGui::Separator();
	ImGui::Text("Motion Blur");
	ImGui::Checkbox("Enable motion blur", &enableBlending);
	ImGui::SliderFloat("Brightness", &motionBlurBrightness, 0.0f, 1.0f);
	ImGui::SliderFloat("Alpha", &motionBlurAlpha, 0.0f, 0.95f);

	ImGui::Separator();
	ImGui::Text("Depth of Field");
	ImGui::SliderFloat("Focal distance", &focalDistance, -50.0f, 0.0f);
	ImGui::SliderFloat("Max distance", &maxDofDistance, 0.0f, 1.0f);
	ImGui::Text("Kernel size:");
	if (ImGui::RadioButton("3x3", shaderManager.pp_dof.mask.size == size3x3))
		shaderManager.pp_dof.mask = { gauss3x3, texIdx3x3, size3x3 };
	ImGui::SameLine();
	if (ImGui::RadioButton("5x5", shaderManager.pp_dof.mask.size == size5x5))
		shaderManager.pp_dof.mask = { gauss5x5, texIdx5x5, size5x5 };
	ImGui::SameLine();
	if (ImGui::RadioButton("7x7", shaderManager.pp_dof.mask.size == size7x7))
		shaderManager.pp_dof.mask = { gauss7x7, texIdx7x7, size7x7 };
	ImGui::SameLine();
	if (ImGui::RadioButton("9x9", shaderManager.pp_dof.mask.size == size9x9))
		shaderManager.pp_dof.mask = { gauss9x9, texIdx9x9, size9x9 };

	ImGui::End();
}

void drawImGuiPanels() {
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 0), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	drawScenePanel();
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 0), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(10, 30));
	drawLightingPanel();
	if (renderer.getType() == Forward) return;
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 0), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(10, 50));
	drawPostProcessingPanel();
}