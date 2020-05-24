#pragma once
#include "BaseMesh.h"
#include "UniformBuffer.h"

class MapTileMesh : public BaseMesh
{
private:
	UniformBuffer PositionMatrixBuffer;

protected:
	void CreateUniformBuffers(VulkanRenderer& Renderer);
	void CreateDescriptorPool(VulkanRenderer& Renderer);
	void CreateDescriptorSets(VulkanRenderer& Renderer);

public:

	glm::vec3 MeshPosition = glm::vec3();
	glm::vec3 MeshRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	MapTileMesh();
	MapTileMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<Texture2D>& textureList);
	~MapTileMesh();

	void Draw(VulkanRenderer& Renderer, int currentFrame);
	void UpdateUniformBuffer(VulkanRenderer& Renderer, PositionMatrix positionMatrix);
	void Destroy(VulkanRenderer& Renderer);
};
