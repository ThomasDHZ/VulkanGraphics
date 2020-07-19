#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "VulkanResources.h"

class UniformBuffer : public VulkanResources
{
private:
	VkDeviceSize BufferSize;
	std::vector<VkBuffer> UniformBuffers;
	std::vector<VkDeviceMemory> UniformBuffersMemory;

	void CreateUniformBuffers(Renderer& renderer);
public:
	UniformBuffer();
	UniformBuffer(Renderer& renderer, VkDeviceSize bufferSize);
	~UniformBuffer();

	void UpdateUniformBuffer(Renderer& renderer, void* UniformObjectData);
	void Destroy(Renderer& renderer);
	VkBuffer GetUniformBuffer(int currentImage) { return UniformBuffers[currentImage]; };
};
