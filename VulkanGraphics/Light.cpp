#include "Light.h"

Light::Light()
{
}

Light::Light(VulkanRenderer& Renderer)
{
}

Light::~Light()
{
}

void Light::UpdateUniformBuffer(VulkanRenderer& Renderer, VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize)
{
	void* UniformData;
	vkMapMemory(*GetDevice(Renderer), UniformBufferMemory, 0, UniformSize, 0, &UniformData);
	memcpy(UniformData, UniformObjectData, UniformSize);
	vkUnmapMemory(*GetDevice(Renderer), UniformBufferMemory);
}
