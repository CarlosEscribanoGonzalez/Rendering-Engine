#include "Scene.h"
#include "SceneManager.h"
#include "GlobalVariables.h"

void SceneManager::initScenes() {
	initDeferred_1();
	initDeferred_2();
	initDeferred_3();
	initDeferred_4();
	initForward();
	currentScene = deferredScene_4; //Default scene
}

void SceneManager::initDeferred_1() {
	deferredScene_1 = new Scene();
	deferredScene_1->addObject(meshManager.cube, shaderManager.geometryPass_procedural, materialManager.cubeMat);
	deferredScene_1->addObject(meshManager.cube, shaderManager.geometryPass_procedural, materialManager.cubeMat);
	ObjectInfo* trainBody = deferredScene_1->addObject(meshManager.trainBody, shaderManager.geometryPass_procedural, materialManager.trainMat);
	deferredScene_1->addObject(meshManager.trainArms, 
		shaderManager.geometryPass_procedural, materialManager.trainMat, trainBody);
	deferredScene_1->addCamera(Camera(glm::vec3(0, 0, 8.0f)));
	deferredScene_1->setPostProcessing(std::vector<PostProcessProgram*> {&shaderManager.pp_edgeDetection});
	deferredScene_1->resizeCameras();
}

void SceneManager::initDeferred_2() {
	deferredScene_2 = new Scene();
	deferredScene_2->addObject(meshManager.suzanne, shaderManager.geometryPass_default, materialManager.suzanneMat);
	deferredScene_2->addLight(Light(1, glm::vec3(1.0, 0.0, 0.0), glm::vec3(-1.0, -1.0, -1.0), 0.75f));
	deferredScene_2->addLight(Light(0, 1, glm::vec3(-3.0f, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)));
	deferredScene_2->addLight(Light(2, 10, glm::vec3(3.0f, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(-1.0f, 0.0, 0.0), 15));
	deferredScene_2->addCamera(Camera(glm::vec3(0, 0, 6.0f)));
	deferredScene_2->setAmbientIntensity(0);
	deferredScene_2->setPostProcessing(std::vector<PostProcessProgram*> {&shaderManager.pp_bloom});
	deferredScene_2->resizeCameras();
}

void SceneManager::initDeferred_3() {
	deferredScene_3 = new Scene();
	deferredScene_3->addObject(meshManager.suzanne, shaderManager.geometryPass_default, materialManager.suzanneMat);
	deferredScene_3->addObject(meshManager.cube, shaderManager.geometryPass_bump, materialManager.cubeMat);
	ObjectInfo* trainBody = deferredScene_3->addObject(meshManager.trainBody, shaderManager.geometryPass_procedural, materialManager.trainMat);
	deferredScene_3->addObject(meshManager.trainArms, shaderManager.geometryPass_discard, 
		materialManager.trainMat, trainBody);
	deferredScene_3->addLight(Light(1, glm::vec3(1.0, 0.0, 0.0), glm::vec3(-1.0, -1.0, -1.0), 0.75f));
	deferredScene_3->addLight(Light(0, 1, glm::vec3(-3.0f, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)));
	deferredScene_3->addLight(Light(2, 10, glm::vec3(3.0f, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(-1.0f, 0.0, 0.0), 15));
	deferredScene_3->addCamera(Camera(glm::vec3(0, 0, 8.0f)));
	deferredScene_3->resizeCameras();
}

void SceneManager::initDeferred_4() {
	deferredScene_4 = new Scene();
	//Objects (mesh - program - material):
	ObjectInfo* suzanne = deferredScene_4->addObject(meshManager.suzanne, 
		shaderManager.geometryPass_default, materialManager.suzanneMat);
	ObjectInfo* camullCube = deferredScene_4->addObject(meshManager.cube, 
		shaderManager.geometryPass_bump, materialManager.cubeMat, suzanne);
	ObjectInfo* trainBody = deferredScene_4->addObject(meshManager.trainBody, 
		shaderManager.geometryPass_procedural, materialManager.trainMat, suzanne);
	deferredScene_4->addObject(meshManager.trainArms, shaderManager.geometryPass_procedural, 
		materialManager.trainMat, trainBody);
	for (int i = 0; i < 10; i++) {
		deferredScene_4->addObject(meshManager.cube, shaderManager.geometryPass_discard, materialManager.cubeMat);
	}
	//Cameras:
	deferredScene_4->addCamera(Camera(glm::vec3(0.0, 0.0, 25.0f)));
	deferredScene_4->addCamera(Camera(glm::vec3(0.0, 0.0, -25.0f), 90));
	deferredScene_4->resizeCameras();
	//Lights
	deferredScene_4->addLight(Light(1, glm::vec3(1.0, 0.0, 0.0), glm::vec3(-1.0), 1));
	deferredScene_4->addLight(Light(0, 10, glm::vec3(0.0), glm::vec3(0.0, 0.0, 1.0)));
	deferredScene_4->addLight(Light(2, 30, glm::vec3(0.0, 0.0, 20.0), glm::vec3(0.0, 1.0, 0.0),
		glm::vec3(0.0, 0.0, -1.0), 45.0f));
	//Post-processing:
	deferredScene_4->setPostProcessing({&shaderManager.pp_edgeDetection, &shaderManager.pp_sobelX, 
		&shaderManager.pp_sobelY,& shaderManager.pp_bloom,& shaderManager.pp_dof,& shaderManager.pp_dof});
}

void SceneManager::initForward() {
	forwardScene = new Scene();
	forwardScene->addObject(meshManager.suzanne, shaderManager.forward_disney, materialManager.suzanneMat);
	forwardScene->addObject(meshManager.cube, shaderManager.forward_bump, materialManager.cubeMat);
	ObjectInfo* trainBody = forwardScene->addObject(meshManager.trainBody, shaderManager.forward_procedural, materialManager.trainMat);
	forwardScene->addObject(meshManager.trainArms, shaderManager.forward_fog, 
		materialManager.trainMat, trainBody);
	forwardScene->addObject(meshManager.cube, shaderManager.forward_default, materialManager.cubeMat);
	forwardScene->addObject(meshManager.cube, shaderManager.forward_discard, materialManager.cubeMat);
	forwardScene->addLight(Light(1, glm::vec3(1.0, 0.0, 0.0), glm::vec3(-1.0, -1.0, -1.0), 0.75f));
	forwardScene->addLight(Light(0, 1, glm::vec3(-3.0f, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)));
	forwardScene->addLight(Light(2, 10, glm::vec3(3.0f, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(-1.0f, 0.0, 0.0), 15));
	forwardScene->addCamera(Camera(glm::vec3(0, 0, 8.0f)));
	forwardScene->addCamera(Camera(glm::vec3(-8.0f, 8.0f, -8.0f)));
	forwardScene->addCamera(Camera(glm::vec3(-15.0f, 0.0f, 0.0f)));
	forwardScene->resizeCameras();
}

void SceneManager::changeScene(Scene* newScene){
	currentScene = newScene;
	currentScene->resizeCameras();
}