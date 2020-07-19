#include "UniformBuffer.h"
#include "VulkanBufferManager.h"

UniformBuffer::UniformBuffer()
{
}

UniformBuffer::UniformBuffer(Renderer& renderer, VkDeviceSize bufferSize)
{
	BufferSize = bufferSize;
	CreateUniformBuffers(renderer);
}

UniformBuffer::~UniformBuffer()
{
}

void UniformBuffer::CreateUniformBuffers(Renderer& renderer)
{
	UniformBuffers.resize(GetSwapChainImageCount(renderer));
	UniformBuffersMemory.resize(GetSwapChainImageCount(renderer));
	for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
	{
		VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), BufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, UniformBuffers[i], UniformBuffersMemory[i]);
	}
}

void UniformBuffer::UpdateUniformBuffer(Renderer& renderer, void* UniformObjectData)
{
	void* UniformData;
	vkMapMemory(*GetDevice(renderer), UniformBuffersMemory[renderer.DrawFrame], 0, BufferSize, 0, &UniformData);
	memcpy(UniformData, UniformObjectData, BufferSize);
	vkUnmapMemory(*GetDevice(renderer), UniformBuffersMemory[renderer.DrawFrame]);
}

void UniformBuffer::Destroy(Renderer& renderer)
{
	for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
	{
		vkDestroyBuffer(*GetDevice(renderer), UniformBuffers[i], nullptr);
		vkFreeMemory(*GetDevice(renderer), UniformBuffersMemory[i], nullptr);

		UniformBuffers[i] = VK_NULL_HANDLE;
		UniformBuffersMemory[i] = VK_NULL_HANDLE;
	}
}