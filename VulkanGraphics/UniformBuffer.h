#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "VulkanResources.h"
#include "VulkanRenderer.h"

class UniformBuffer : public VulkanResources
{
private:
	VkDeviceSize BufferSize;
	std::vector<VkBuffer> UniformBuffers;
	std::vector<VkDeviceMemory> UniformBuffersMemory;

	void CreateUniformBuffers(VulkanRenderer& Renderer);
public:
	UniformBuffer();
	UniformBuffer(VulkanRenderer& Renderer, VkDeviceSize bufferSize);
	~UniformBuffer();

	void UpdateUniformBuffer(VulkanRenderer& Renderer, void* UniformObjectData);
	void Destroy(VulkanRenderer& Renderer);
	VkBuffer GetUniformBuffer(int currentImage) { return UniformBuffers[currentImage]; };
};

