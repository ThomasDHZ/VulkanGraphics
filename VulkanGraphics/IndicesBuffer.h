#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "VulkanRenderer.h"

class IndicesBuffer
{
private:
	unsigned int IndiceCount = 0;
	VkBuffer indiceBuffer = VK_NULL_HANDLE;
	VkDeviceMemory IndiceBufferMemory = VK_NULL_HANDLE;

public:

	IndicesBuffer();
	IndicesBuffer(VulkanRenderer& renderer, const std::vector<uint16_t>& indicesdata);
	~IndicesBuffer();

	void Destory(VulkanRenderer& renderer);

	unsigned int GetIndiceCount() { return IndiceCount; }
	VkBuffer GetIndiceBuffer() { return indiceBuffer; }
};
