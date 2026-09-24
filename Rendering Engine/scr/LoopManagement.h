#pragma once
#include "GlobalVariables.h"

void swizzlePoints(float* points, int length) {
	float initValue = points[0];
	for (int i = 0; i < length - 1; i++) {
		points[i] = points[i + 1];
	}
	points[length - 1] = initValue;
}

glm::mat4 catmullMat = glm::mat4(0, -1, 2, -1, 2, 0, -5, 3, 0, 1, 4, -3, 0, 0, -1, 1);
const int numPoints = 8;
float pointsX[numPoints] = { -4.0, -4.5, 0.0, 4.5, 4.0, 4.5, 0.0, -4.5 };
float pointsY[numPoints] = { 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0 };
float pointsZ[numPoints] = { 0.0, 4.5, 4.0, 4.5, 0.0, -4.5, -4.0, -4.5 };
float currentT = 0;
void calculateCatmullRom(glm::mat4& modelMat) {
	glm::vec4 baseMult = glm::vec4(1, currentT, pow(currentT, 2), pow(currentT, 3)) * 0.5f * catmullMat;
	float currentX = glm::dot(baseMult, glm::vec4(pointsX[0], pointsX[1], pointsX[2], pointsX[3]));
	float currentY = glm::dot(baseMult, glm::vec4(pointsY[0], pointsY[1], pointsY[2], pointsY[3]));
	float currentZ = glm::dot(baseMult, glm::vec4(pointsZ[0], pointsZ[1], pointsZ[2], pointsZ[3]));
	modelMat[3].x = currentX;
	modelMat[3].y = currentY;
	modelMat[3].z = currentZ;
	currentT += objMovementSpeed;
	if (currentT > 1.0) {
		currentT = 0;
		swizzlePoints(pointsX, numPoints);
		swizzlePoints(pointsY, numPoints);
		swizzlePoints(pointsZ, numPoints);
	}
}

void idleFunc() {
	if (pause) {
		glutPostRedisplay();
		return;
	}
	static float angle = 0.0f;
	angle = (angle > PI * 2.0f) ? 0 : angle + 0.01f;
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0, 0.0, 2.0));
	model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	sceneManager.getCurrentScene()->setModel(0, model);
	model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0.0f));
	calculateCatmullRom(model);
	sceneManager.getCurrentScene()->setModel(1, model);
	model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, -1.0));
	model = glm::translate(model, glm::vec3(4.0, 0.0, 0.0));
	model = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0));
	sceneManager.getCurrentScene()->setModel(2, model);
	model = glm::rotate(glm::mat4(1.0f), angle * 5, glm::vec3(0.0, 1.0, 0.0));
	sceneManager.getCurrentScene()->setModel(3, model);
	std::srand(RAND_SEED);
	for (unsigned int i = 4; i < 14; i++)
	{
		float size = float(std::rand() % 3 + 1);

		glm::vec3 axis(glm::vec3(float(std::rand() % 2),
			float(std::rand() % 2), float(std::rand() % 2)));
		if (glm::all(glm::equal(axis, glm::vec3(0.0f))))
			axis = glm::vec3(1.0f);

		float trans = float(std::rand() % 7 + 3) * 1.00f + 0.5f;
		glm::vec3 transVec = axis * trans;
		transVec.x *= (std::rand() % 2) ? 1.0f : -1.0f;
		transVec.y *= (std::rand() % 2) ? 1.0f : -1.0f;
		transVec.z *= (std::rand() % 2) ? 1.0f : -1.0f;

		model = glm::rotate(glm::mat4(1.0f), angle * 2.0f * size, axis);
		model = glm::translate(model, transVec);
		model = glm::rotate(model, angle * 2.0f * size, axis);
		model = glm::scale(model, glm::vec3(1.0f / (size * 0.7f)));
		sceneManager.getCurrentScene()->setModel(i, model);
	}
	sceneManager.getCurrentScene()->updateModels();
	glutPostRedisplay();
}