#pragma once
#include <vector>
#include <string>

struct MeshInfo {
	unsigned int nTrianglesIndex;
	unsigned int nVertex;
	std::vector<unsigned int> triangleIndex;
	std::vector<float> vertexPos;
	std::vector<float> vertexColor;
	std::vector<float> vertexNormal;
	std::vector<float> vertexTexCoord;
	std::vector<float> vertexTangent;
	
	bool operator<(const MeshInfo& other) const {
		return nVertex < other.nVertex;
	}
};

class MeshManager {
private:
	MeshInfo importModel(std::string name, int componentIndex = 0); //Imports a model from a file

	MeshInfo importModel(const int nTrianglesIndex, const int nVertex, const unsigned int* tIndex, const float* vPos,
		const float* vColor, const float* vNormal, const float* vTexCoord, const float* vTangent); //Imports from .h

public:
	MeshInfo plane;
	MeshInfo cube;
	MeshInfo suzanne;
	MeshInfo trainBody;
	MeshInfo trainArms;

	void initMeshes();
};