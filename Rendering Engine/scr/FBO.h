#include <GL/glew.h>
#define SOLVE_FGLUT_WARNING
#include <GL/freeglut.h>
#pragma once

class FBO {
public:
	unsigned int id;
	unsigned int colorBuffTexId;
	unsigned int depthBuffTexId;
	unsigned int posBuffTexId;
	unsigned int normalBuffTexId;
	unsigned int specularBuffTexId;
	unsigned int emissiveBuffTexId;
	unsigned int litBuffTexId;

	void init();

	void resize(unsigned int w, unsigned int h) const;

	void destroy() const;
};