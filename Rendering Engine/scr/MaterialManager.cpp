#include "MaterialManager.h"
#include <iostream>
#include "auxiliar.h"

void MaterialManager::loadTextures() {
	//Checking anisotropic filter availability and max value
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropicAmount);
		std::cout << "Filtro anisotrópico disponible. Tamaño máximo: " << anisotropicAmount << std::endl;
	}
	//Texture loading:
	colorTexId = loadTex("../img/color2.png");
	emiTexId = loadTex("../img/emissive.png");
	normalTexId = loadTex("../img/normal.png");
	specularTexId = loadTex("../img/specMap.png");
	defaultSpecTexId = loadTex("../img/defaultSpec.png");
	suzanneTexId = loadTex("../img/SuzanneColor.jpg");
	int randTexIndex = rand() % 3 + 1;
	char path[50];
	sprintf_s(path, "../img/train%d.png", randTexIndex);
	trainTexId = loadTex(path);
}

void MaterialManager::initMaterials() {
	loadTextures();
	cubeMat = { colorTexId, specularTexId, normalTexId, emiTexId };
	suzanneMat = { suzanneTexId, defaultSpecTexId };
	trainMat = { trainTexId, defaultSpecTexId };
}

void MaterialManager::destroyMaterials() {
	glDeleteTextures(1, &colorTexId);
	glDeleteTextures(1, &emiTexId);
	glDeleteTextures(1, &normalTexId);
	glDeleteTextures(1, &specularTexId);
	glDeleteTextures(1, &trainTexId);
	glDeleteTextures(1, &defaultSpecTexId);
}

unsigned int MaterialManager::loadTex(const char* fileName) {
	unsigned char* map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);
	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< fileName << std::endl;
		exit(-1);
	}
	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);
	delete[] map;
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//Anisotropic filter:	
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropicAmount);
	return texId;
}