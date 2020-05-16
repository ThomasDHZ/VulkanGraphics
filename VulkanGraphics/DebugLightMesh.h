#pragma once
#include "BaseMesh.h"
#include "UniformBuffer.h"
#include "Mesh.h"

class DebugLightMesh : public BaseMesh
{
private:

protected:
	void CreateUniformBuffers(VulkanRenderer& Renderer);
	void CreateDescriptorPool(VulkanRenderer& Renderer);
	void CreateDescriptorSets(VulkanRenderer& Renderer);

public:

	UniformBuffer PositionMatrixBuffer;
	UniformBuffer LighterBuffers;

	DebugLightMesh();
	DebugLightMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
	DebugLightMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices);
	~DebugLightMesh();

	void Draw(VulkanRenderer& Renderer, int currentFrame);
	void UpdateUniformBuffer(VulkanRenderer& Renderer, PositionMatrix positionMatrix, Lighter lighter, int currentImage);
	void Destroy(VulkanRenderer& Renderer);
};

