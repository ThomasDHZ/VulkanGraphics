#pragma once
#include "BaseMesh.h"
#include "Renderer.h"
#include "UniformBuffer.h"

class WireFrameMesh : public BaseMesh
{
private:

protected:
	void CreateUniformBuffers(Renderer& renderer);
	void CreateDescriptorPool(Renderer& renderer);
	void CreateDescriptorSets(Renderer& renderer);

public:

	std::string MeshName;

	UniformBuffer PositionMatrixBuffer;

	glm::vec3 MeshPosition = glm::vec3();
	glm::vec3 MeshRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	WireFrameMesh();
	WireFrameMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
	~WireFrameMesh();

	void UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix);
	void Destroy(Renderer& renderer);

	float* GetMeshPosPtr() { return &MeshPosition.x; };
};

