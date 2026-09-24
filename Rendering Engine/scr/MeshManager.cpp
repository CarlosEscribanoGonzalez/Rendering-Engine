#include "MeshManager.h"
#include "BOX.h"
#include "suzanne.h"
#include "PLANE.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void MeshManager::initMeshes() {
	plane = importModel(0, planeNVertex, nullptr, planeVertexPos, nullptr, nullptr, nullptr, nullptr);
	cube = importModel(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, cubeVertexPos,
		cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);
	suzanne = importModel(SUZANNE_NUM_FACES, SUZANNE_NUM_VERTICES, suzanneFaces, suzanneVertexPos,
		fillSuzanneVertexColor(), suzanneVertexNormal, generateTriplanar(SUZANNE_NUM_VERTICES, 1.0f).data(), nullptr);
	trainBody = importModel("Train.fbx", 1);
	trainArms = importModel("Train.fbx", 0);
}

MeshInfo MeshManager::importModel(std::string name, int componentIndex) {
	std::string path = "../models/" + name;
	const struct aiScene* scene = aiImportFile(path.c_str(),
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals);
	if (!scene) {
		fprintf(stderr, "Error loading model: %s\n", aiGetErrorString());
		return MeshInfo{};
	}
	MeshInfo m = {};
	const struct aiMesh* mesh = scene->mMeshes[componentIndex];
	m.nVertex = mesh->mNumVertices;
	m.nTrianglesIndex = mesh->mNumFaces;
	m.vertexPos = std::vector<float>(3 * m.nVertex);
	m.vertexNormal = std::vector<float>(3 * m.nVertex);
	m.vertexTexCoord = std::vector<float>(2 * m.nVertex);
	m.vertexTangent = std::vector<float>(3 * m.nVertex);
	m.vertexColor = std::vector<float>(3 * m.nVertex);
	m.triangleIndex = std::vector<unsigned int>(3 * m.nTrianglesIndex);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D pos = mesh->mVertices[i];
		aiVector3D norm = mesh->mNormals[i];
		aiVector3D vec0;
		vec0.x = vec0.y = vec0.z = 0;
		aiVector3D tex = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : vec0;
		aiVector3D tan = mesh->HasTangentsAndBitangents() ? mesh->mTangents[i] : vec0;
		m.vertexPos[i * 3 + 0] = pos.x;
		m.vertexPos[i * 3 + 1] = pos.y;
		m.vertexPos[i * 3 + 2] = pos.z;
		m.vertexNormal[i * 3 + 0] = norm.x;
		m.vertexNormal[i * 3 + 1] = norm.y;
		m.vertexNormal[i * 3 + 2] = norm.z;
		m.vertexTexCoord[i * 2 + 0] = tex.x;
		m.vertexTexCoord[i * 2 + 1] = tex.y;
		m.vertexTangent[i * 3 + 0] = tan.x;
		m.vertexTangent[i * 3 + 1] = tan.y;
		m.vertexTangent[i * 3 + 2] = tan.z;
		m.vertexColor[i * 3 + 0] = 1.0f;
		m.vertexColor[i * 3 + 1] = 1.0f;
		m.vertexColor[i * 3 + 2] = 1.0f;
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const struct aiFace face = mesh->mFaces[i];
		if (face.mNumIndices == 3) {
			m.triangleIndex[i * 3 + 0] = face.mIndices[0];
			m.triangleIndex[i * 3 + 1] = face.mIndices[1];
			m.triangleIndex[i * 3 + 2] = face.mIndices[2];
		}
	}
	return m;
}

MeshInfo MeshManager::importModel(const int nTrianglesIndex, const int nVertex, const unsigned int* tIndex, const float* vPos,
	const float* vColor, const float* vNormal, const float* vTexCoord, const float* vTangent) {
	MeshInfo m = {};
	m.nTrianglesIndex = nTrianglesIndex;
	m.nVertex = nVertex;
	m.triangleIndex.assign(tIndex, tIndex + 3 * m.nTrianglesIndex);
	m.vertexPos.assign(vPos, vPos + 3 * m.nVertex);
	if (vColor) m.vertexColor.assign(vColor, vColor + 3 * m.nVertex);
	if (vNormal) m.vertexNormal.assign(vNormal, vNormal + 3 * m.nVertex);
	if (vTexCoord) m.vertexTexCoord.assign(vTexCoord, vTexCoord + 2 * m.nVertex);
	if (vTangent) m.vertexTangent.assign(vTangent, vTangent + 3 * m.nVertex);
	return m;
}