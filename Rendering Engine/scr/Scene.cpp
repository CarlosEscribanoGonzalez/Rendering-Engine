#include "Scene.h"
#include "GlobalVariables.h"

void Scene::addLight(const Light& light) { lights.push_back(light); }

void Scene::addCamera(const Camera& cam) { cameras.push_back(cam); }

ObjectInfo* Scene::addObject(const MeshInfo& meshInfo, const GeometryProgram& programInfo, 
	const Material& mat, ObjectInfo* parent) {
	ObjectInfo* obj = new ObjectInfo();
	obj->programInfo = programInfo;
	obj->vaoInfo = vaoManager.getVAO(meshInfo, programInfo);
	obj->mat = mat;
	obj->model = glm::mat4(1.0f);
	if (parent) parent->children.insert(obj);
	else rootObjects.push_back(obj);
	localModels[obj] = glm::mat4(1.0);
	objects.push_back(obj);
	return obj;
}

void Scene::setModel(int objIndex, glm::mat4 newModel) { 
	if (objIndex >= objects.size()) return;
	localModels[objects[objIndex]] = newModel;
}

void Scene::updateModels() {
	for (ObjectInfo* obj : rootObjects) {
		updateHierarchy(obj, glm::mat4(1.0));
	}
}

void Scene::updateHierarchy(ObjectInfo* obj, glm::mat4 parentModel) {
	obj->model = parentModel * localModels[obj];
	for (ObjectInfo* child : obj->children) {
		updateHierarchy(child, obj->model);
	}
}

void Scene::resizeCameras() {
	for (Camera& cam : cameras) cam.resize();
}

void Scene::changeCamera(int idx) {
	if (idx == -1) {
		if (++camIndex == cameras.size()) camIndex = 0;
	}
	else {
		idx = std::max(std::min(idx, (int)cameras.size() - 1), 0);
		camIndex = idx;
	}
}