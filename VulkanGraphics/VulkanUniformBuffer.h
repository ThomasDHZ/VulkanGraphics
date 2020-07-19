#pragma once
#include <vulkan\vulkan_core.h>
#include "VulkanRenderer.h"
#include "NewVulkanBufferManager.h"

class VulkanUniformBuffer
{
private:
	VkDeviceSize BufferSize;
	std::vector<VkBuffer> UniformBuffers;
	std::vector<VkDeviceMemory> UniformBuffersMemory;

	void CreateUniformBuffers(VulkanRenderer& renderer);
public:
	VulkanUniformBuffer();
	VulkanUniformBuffer(VulkanRenderer& renderer, VkDeviceSize bufferSize);
	~VulkanUniformBuffer();

	void UpdateUniformBuffer(VulkanRenderer& renderer, void* UniformObjectData);
	void Destroy(VulkanRenderer& renderer);
	VkBuffer GetUniformBuffer(int currentImage) { return UniformBuffers[currentImage]; };
};
