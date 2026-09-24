#pragma once
#include <GL/glew.h>
#define SOLVE_FGLUT_WARNING
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include "GlobalVariables.h"

class Light {
public:
	int type = 0; //0 -> point; 1 -> directional; 2 -> focal
	float intensity = 1;
	glm::vec3 position = glm::vec3(0.0, 0.0, 6.0);
	glm::vec3 diffuse = glm::vec3(1.0);
	glm::vec3 direction = glm::vec3(-1.0, -1.0, 0.0);
	float angle = 15;
	glm::vec3 specular = glm::vec3(1.0);
	float dmax = 25;
	float alpha = 500;
	float dmin = 1;
	float d0 = 3;

#pragma region Constructores
	Light() {}

	Light(int t, glm::vec3 color, glm::vec3 dir, float intens) : //Useful for directional lights
		type(t),
		intensity(intens),
		direction(dir),
		diffuse(color),
		specular(color)
	{
	}

	Light(int t, float intens, glm::vec3 pos, glm::vec3 color) : //Useful for point lights
		type(t),
		intensity(intens),
		position(pos),
		diffuse(color),
		specular(color)
	{
	}

	Light(int t, float intens, glm::vec3 pos, glm::vec3 color, glm::vec3 dir, float angle) : //Useful for focal lights
		type(t),
		intensity(intens),
		position(pos),
		diffuse(color),
		specular(color),
		direction(dir),
		angle(angle)
	{
	}

	Light(int t, float intens, glm::vec3 pos, glm::vec3 color, glm::vec3 dir, float angle,
		float alpha, float dmax, float dmin, float d0) :
		type(t),
		intensity(intens),
		position(pos),
		diffuse(color),
		specular(color),
		direction(dir),
		angle(angle),
		alpha(alpha),
		dmax(dmax),
		dmin(dmin),
		d0(d0)
	{
	}
#pragma endregion

	void update(glm::vec3 movement, float intensityVariation) {
		intensity = std::max(intensity + intensityVariation * lightIntensityVariationSpeed, 0.0f);
		position += movement * lightMovementSpeed;
	}
};