#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GlobalVariables.h"

class Camera {
private:
	//VIEW:
	glm::mat4 view = glm::mat4(1.0);
	glm::vec3 camPos = glm::vec3(0.0); //In world coordinates
	glm::vec3 camForward = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 camRight = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
	//PROJ:
	glm::mat4 proj = glm::mat4(1.0f);
	float fov;

public:
	inline const glm::mat4& getView() const { return view; }

	inline const glm::mat4& getProj() const { return proj; }

#pragma region Constructores
	Camera() : Camera(glm::vec3(0.0), glm::vec3(0.0, 0.0, 1.0), 60) {}

	Camera(glm::vec3 camPos, float fov = 60) : Camera(camPos, glm::vec3(0.0), fov) {}

	Camera(glm::vec3 camPos, glm::vec3 lookPoint, float fov = 60) : camPos(camPos), fov(fov) {
		camForward = glm::normalize(lookPoint - camPos);
		camRight = glm::cross(camForward, glm::vec3(0.0, 1.0, 0.0));
		camUp = glm::cross(camRight, camForward);
		updateView();
		resize();
	}
#pragma endregion

	void updateView() {
		view = glm::lookAt(camPos, camPos + camForward, camUp);
	}

	void resize() {
		proj = glm::perspective(glm::radians(fov), aspectRatio, NEAR_DIST, FAR_DIST);
	}

	void translate(glm::vec3 dir) {
		dir = glm::normalize(dir.x * camRight + dir.y * camUp + dir.z * camForward);
		camPos += dir * camMovementSpeed;
		updateView();
	}

	void rotate(glm::vec3 rotDir, bool orbitalRotation = false) {
		rotDir = glm::normalize(rotDir.x* camRight + rotDir.y * camUp + rotDir.z * camForward);
		glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0), camRotationSpeed, rotDir);
		camForward = rotationMat * glm::vec4(camForward, 0.0);
		camRight = rotationMat * glm::vec4(camRight, 0.0);
		camUp = rotationMat * glm::vec4(camUp, 0.0);
		if (orbitalRotation) camPos = rotationMat * glm::vec4(camPos, 1.0);
		updateView();
	}
};