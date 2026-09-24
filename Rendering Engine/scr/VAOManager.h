#pragma once
#include <map>
#include "MeshManager.h"
#include "ShaderManager.h"

struct VAOInfo {
	unsigned int vao;
	unsigned int posVBO;
	unsigned int colorVBO;
	unsigned int normalVBO;
	unsigned int texCoordVBO;
	unsigned int tangentVBO;
	unsigned int triangleIndexVBO;
	MeshInfo mesh;
};

class VAOManager {
private:
	std::vector<VAOInfo*> vaos;
	std::map<std::pair<MeshInfo, GeometryProgram>, VAOInfo*> vaoMap_geometry;
	std::map<std::pair<MeshInfo, LightingProgram>, VAOInfo*> vaoMap_lighting;

public:
	VAOInfo& getVAO(const MeshInfo& meshInfo, const GeometryProgram& programInfo);

	VAOInfo* initVAO(const MeshInfo& meshInfo, const GeometryProgram& programInfo);

	void destroyVAOs();
};