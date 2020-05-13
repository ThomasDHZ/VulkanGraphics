#include "UniformBuffer.h"
#include "VulkanBufferManager.h"

UniformBuffer::UniformBuffer()
{
}

UniformBuffer::UniformBuffer(VulkanRenderer& Renderer, VkDeviceSize bufferSize)
{
	BufferSize = bufferSize;
	CreateUniformBuffers(Renderer);
}

UniformBuffer::~UniformBuffer()
{
}

void UniformBuffer::CreateUniformBuffers(VulkanRenderer& Renderer)
{
	UniformBuffers.resize(GetSwapChainImageCount(Renderer));
	UniformBuffersMemory.resize(GetSwapChainImageCount(Renderer));
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), BufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, UniformBuffers[i], UniformBuffersMemory[i]);
	}
}

void UniformBuffer::UpdateUniformBuffer(VulkanRenderer& Renderer, void* UniformObjectData, int currentImage)
{
	void* UniformData;
	vkMapMemory(*GetDevice(Renderer), UniformBuffersMemory[currentImage], 0, BufferSize, 0, &UniformData);
	memcpy(UniformData, UniformObjectData, BufferSize);
	vkUnmapMemory(*GetDevice(Renderer), UniformBuffersMemory[currentImage]);
}

void UniformBuffer::Destroy(VulkanRenderer& Renderer)
{
	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		vkDestroyBuffer(*GetDevice(Renderer), UniformBuffers[i], nullptr);
		vkFreeMemory(*GetDevice(Renderer), UniformBuffersMemory[i], nullptr);

		UniformBuffers[i] = VK_NULL_HANDLE;
		UniformBuffersMemory[i] = VK_NULL_HANDLE;
	}
}
