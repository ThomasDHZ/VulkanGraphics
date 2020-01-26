#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "VulkanBufferManager.h"

class IndexBufferObject
{
private:
	VkDevice Device;
	VkPhysicalDevice PhysicalDevice;
	VkBuffer IndexBuffer;
	VkDeviceMemory IndexBufferMemory;

public:
	IndexBufferObject()
	{

	}

	IndexBufferObject(int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<uint16_t> IndexData, VkCommandPool& CommandPool, VkQueue& GraphicsQueue)
	{
		Device = device;
		PhysicalDevice = physicalDevice;

		VkDeviceSize bufferSize = sizeof(IndexData[0]) * IndexData.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(Device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, IndexData.data(), (size_t)bufferSize);
		vkUnmapMemory(Device, stagingBufferMemory);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, IndexBuffer, IndexBufferMemory);

		VulkanBufferManager::CopyBuffer(device, physicalDevice, stagingBuffer, IndexBuffer, bufferSize, CommandPool, GraphicsQueue);

		vkDestroyBuffer(Device, stagingBuffer, nullptr);
		vkFreeMemory(Device, stagingBufferMemory, nullptr);
	}

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create buffer.");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanBufferManager::FindMemoryType(PhysicalDevice, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate buffer memory.");
		}

		vkBindBufferMemory(Device, buffer, bufferMemory, 0);
	}

	void Destroy()
	{
		vkDestroyBuffer(Device, IndexBuffer, nullptr);
		vkFreeMemory(Device, IndexBufferMemory, nullptr);
	}

	VkBuffer GetIndexBuffer() { return IndexBuffer; }
};

