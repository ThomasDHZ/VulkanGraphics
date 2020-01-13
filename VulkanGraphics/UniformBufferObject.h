#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "VulkanBufferManager.h"

template<class T>
class UniformBufferObject
{
private:
	VkDevice Device;
	VkPhysicalDevice PhysicalDevice;
	std::vector<VkBuffer> ShaderBuffer;
	std::vector<VkDeviceMemory> ShaderBufferMemory;
public:
	UniformBufferObject()
	{

	}

	UniformBufferObject(int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice)
	{
		Device = device;
		PhysicalDevice = physicalDevice;

		ShaderBuffer.resize(SwapChainSize);
		ShaderBufferMemory.resize(SwapChainSize);

		for (size_t x = 0; x < SwapChainSize; x++)
		{
			CreateBuffer(sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ShaderBuffer[x], ShaderBufferMemory[x]);
		}
	}

	void UpdateShaderBuffer(T StructData, int frame)
	{
		void* data;
		vkMapMemory(Device, ShaderBufferMemory[frame], 0, sizeof(StructData), 0, &data);
		memcpy(data, &StructData, sizeof(StructData));
		vkUnmapMemory(Device, ShaderBufferMemory[frame]);
	}

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanBufferManager::FindMemoryType(PhysicalDevice, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(Device, buffer, bufferMemory, 0);
	}

	void CleanUp(int SwapChainImageSize)
	{
		for (size_t x = 0; x < SwapChainImageSize; x++)
		{
			vkDestroyBuffer(Device, ShaderBuffer[x], nullptr);
			vkFreeMemory(Device, ShaderBufferMemory[x], nullptr);
		}
	}

	std::vector<VkBuffer> GetShaderBuffer() { return ShaderBuffer; }
	std::vector<VkDeviceMemory> GetShaderBufferMemory() { return ShaderBufferMemory; }
};
