#pragma once
#include "VulkanResources.h"

class Light : public VulkanResources
{
private:
public:
	Light();
	Light(VulkanRenderer& Renderer);
	~Light();

	void UpdateUniformBuffer(VulkanRenderer& Renderer, VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize);
};

