#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "Vertex.h"
#include "VulkanRenderer.h"

class VertexBuffer
{
private:
	unsigned int VertexCount = 0;
	VkBuffer vertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory VertexBufferMemory = VK_NULL_HANDLE;

public:

	VertexBuffer();
	VertexBuffer(VulkanRenderer& renderer, const std::vector<Vertex>& vertexdata);
	~VertexBuffer();

	void Destory(VulkanRenderer& renderer);

	unsigned int GetVertexCount() { return VertexCount; }
	VkBuffer GetVertexBuffer() { return vertexBuffer; }
};

