#include "VAOManager.h"

VAOInfo& VAOManager::getVAO(const MeshInfo& meshInfo, const GeometryProgram& programInfo) {
	auto key = std::make_pair(meshInfo, programInfo);
	if (vaoMap_geometry.count(key) == 0) {
		vaoMap_geometry[key] = initVAO(meshInfo, programInfo);
	}
	return *vaoMap_geometry[std::make_pair(meshInfo, programInfo)];
}

VAOInfo* VAOManager::initVAO(const MeshInfo& meshInfo, const GeometryProgram& programInfo) {
	VAOInfo* vao = new VAOInfo();
	vao->mesh = meshInfo;
	glGenVertexArrays(1, &vao->vao);
	glBindVertexArray(vao->vao);

	if (programInfo.inPos != -1)
	{
		glGenBuffers(1, &vao->posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vao->posVBO);
		glBufferData(GL_ARRAY_BUFFER, meshInfo.nVertex * sizeof(float) * 3,
			meshInfo.vertexPos.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(programInfo.inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(programInfo.inPos);
	}
	if (programInfo.inColor != -1)
	{
		glGenBuffers(1, &vao->colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vao->colorVBO);
		glBufferData(GL_ARRAY_BUFFER, meshInfo.nVertex * sizeof(float) * 3,
			meshInfo.vertexColor.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(programInfo.inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(programInfo.inColor);
	}
	if (programInfo.inNormal != -1)
	{
		glGenBuffers(1, &vao->normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vao->normalVBO);
		glBufferData(GL_ARRAY_BUFFER, meshInfo.nVertex * sizeof(float) * 3,
			meshInfo.vertexNormal.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(programInfo.inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(programInfo.inNormal);
	}
	if (programInfo.inTexCoord != -1)
	{
		glGenBuffers(1, &vao->texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vao->texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, meshInfo.nVertex * sizeof(float) * 2,
			meshInfo.vertexTexCoord.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(programInfo.inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(programInfo.inTexCoord);
	}
	if (programInfo.inTangent != -1)
	{
		glGenBuffers(1, &vao->tangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vao->tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, meshInfo.nVertex * sizeof(float) * 3,
			meshInfo.vertexTangent.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(programInfo.inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(programInfo.inTangent);
	}

	glGenBuffers(1, &vao->triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshInfo.nTrianglesIndex * sizeof(unsigned int) * 3,
		meshInfo.triangleIndex.data(), GL_STATIC_DRAW);
	vaos.push_back(vao);
	return vao;
}

void VAOManager::destroyVAOs() {
	for (VAOInfo* vao : vaos) {
		glDeleteBuffers(1, &vao->posVBO);
		glDeleteBuffers(1, &vao->colorVBO);
		glDeleteBuffers(1, &vao->normalVBO);
		glDeleteBuffers(1, &vao->texCoordVBO);
		glDeleteBuffers(1, &vao->tangentVBO);
		glDeleteBuffers(1, &vao->triangleIndexVBO);
		glDeleteVertexArrays(1, &vao->vao);
	}
}