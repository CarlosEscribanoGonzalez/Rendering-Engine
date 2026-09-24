#include "auxiliar.h"
#include "FBO.h"
#include "Light.h"
#include "Camera.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "VAOManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GlobalVariables.h"
#include "InputManagement.h"
#include "LoopManagement.h"
#include "Renderer.h"

#include <GL/glew.h>
#define SOLVE_FGLUT_WARNING
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdlib>
#include <map>
#include <string>

// CB declaration 
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);

// Initialization and destruction functions
void initContext(int argc, char** argv);
void initOGL();
void destroy();
void swizzlePoints(float* points, int length); 
void calculateCatmullRom(glm::mat4& modelMat);

int main(int argc, char** argv)
{
#ifdef _WIN32
	std::locale::global(std::locale("spanish")); // Spanish accents
#endif
	initContext(argc, argv);
	initOGL();
	shaderManager.initShaders();
	meshManager.initMeshes();
	materialManager.initMaterials(); 
	sceneManager.initScenes();
	sceneManager.changeScene(sceneManager.deferredScene_4); //Default scene
	renderer.init(Deferred, &shaderManager.lightingPass_disney);
	//renderer.init(Forward);
	glutMainLoop();
	destroy();
	return 0;
}

void initContext(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prácticas OGL");
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte* oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
}

void initOGL() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void destroy() {
	shaderManager.destroyShaders();
	vaoManager.destroyVAOs();
	materialManager.destroyMaterials();
	renderer.destroy();
}

void renderFunc() {
	renderer.render();
}

void resizeFunc(int width, int height) {
	glViewport(0, 0, width, height);
	aspectRatio = (float)width / height;
	sceneManager.getCurrentScene()->resizeCameras();
	renderer.resize(width, height);
	glutPostRedisplay();
}