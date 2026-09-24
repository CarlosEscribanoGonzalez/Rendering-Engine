#pragma once
#include "Camera.h"
#include "Light.h"
#include <vector>
#include <set>
#include <map>
#include "ShaderManager.h"
#include "VAOManager.h"
#include "MaterialManager.h"

struct ObjectInfo {
	GeometryProgram programInfo;
	VAOInfo vaoInfo;
	Material mat;
	glm::mat4 model;
	std::set<ObjectInfo*> children;
};

class Scene {
private:
	std::vector<Camera> cameras;
	int camIndex = 0;
	std::vector<Light> lights;
	float ambientIntensity = 0.2f;
	std::vector<ObjectInfo*> objects;
	std::vector<PostProcessProgram*> ppPrograms;
	//Hierarchical tree:
	std::vector<ObjectInfo*> rootObjects;
	std::map<ObjectInfo*, glm::mat4> localModels;

	void updateHierarchy(ObjectInfo* obj, glm::mat4 parentModel);

public:
	void addLight(const Light& light);

	void addCamera(const Camera& cam);

	ObjectInfo* addObject(const MeshInfo& meshInfo, const GeometryProgram& programInfo, 
		const Material& mat, ObjectInfo* parent = nullptr);

	void setModel(int objIndex, glm::mat4 newModel);

	void updateModels();

	void resizeCameras();

	void changeCamera(int idx = -1);

#pragma region Getters&Setters
	const float& getAmbientIntensity() { return ambientIntensity; }

	const void setAmbientIntensity(float value) { ambientIntensity = value; }

	const std::vector<ObjectInfo*> getObjects() { return objects; }

	std::vector<Light>& getLights() { return lights; }

	Camera& getCurrentCamera() { return cameras[camIndex]; }

	std::vector<PostProcessProgram*> getPostProcessing() { return ppPrograms; }

	void setPostProcessing(std::vector<PostProcessProgram*> postProcessing) { ppPrograms = postProcessing; }
#pragma endregion
};