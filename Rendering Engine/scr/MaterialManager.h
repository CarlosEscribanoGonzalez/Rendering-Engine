#pragma once
#include <GL/glew.h>

struct Material {
	unsigned int colorId;
	unsigned int specularId;
	unsigned int normalId;
	unsigned int emiId;
};

class MaterialManager {
private:
	GLfloat anisotropicAmount;
	//Textures
	unsigned int colorTexId;
	unsigned int emiTexId;
	unsigned int normalTexId;
	unsigned int specularTexId;
	unsigned int trainTexId;
	unsigned int defaultSpecTexId;
	unsigned int suzanneTexId;

	void loadTextures();
	unsigned int loadTex(const char* fileName);

public:
	//Materials
	Material suzanneMat;
	Material cubeMat;
	Material trainMat;

	void initMaterials();

	void destroyMaterials();
};