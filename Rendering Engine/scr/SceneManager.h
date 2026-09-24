#pragma once
class Scene;

class SceneManager {
private:
	Scene* currentScene;

	void initDeferred_1();
	void initDeferred_2();
	void initDeferred_3();
	void initDeferred_4();
	void initForward();

public:
	Scene* deferredScene_1;
	Scene* deferredScene_2;
	Scene* deferredScene_3;
	Scene* deferredScene_4;
	Scene* forwardScene;

	void initScenes();

	void changeScene(Scene* newScene);

	Scene* getCurrentScene() { return currentScene; }
};