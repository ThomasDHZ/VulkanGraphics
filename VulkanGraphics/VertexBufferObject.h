#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "VulkanBufferManager.h"

template<class T>
class VertexBufferObject
{
private:
	VkDevice Device;
	VkPhysicalDevice PhysicalDevice;
	VkBuffer VertexBuffer;
	VkDeviceMemory VertexBufferMemory;
public:
	VertexBufferObject()
	{

	}

	VertexBufferObject(int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<T> VertexData, VkCommandPool& CommandPool, VkQueue& GraphicsQueue)
	{
		Device = device;
		PhysicalDevice = physicalDevice;

		VkDeviceSize bufferSize = sizeof(VertexData[0]) * VertexData.size() * 2;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(Device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, VertexData.data(), (size_t)bufferSize);


		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferMemory);

		VulkanBufferManager::CopyBuffer(device, physicalDevice, stagingBuffer, VertexBuffer, bufferSize, CommandPool, GraphicsQueue);

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

	void Destory()
	{
		vkDestroyBuffer(Device, VertexBuffer, nullptr);
		vkFreeMemory(Device, VertexBufferMemory, nullptr);
	}

	VertexBufferObject& operator=(const VertexBufferObject& rhs)
	{
		Device = rhs.Device;
		PhysicalDevice = rhs.PhysicalDevice;
		VertexBuffer = rhs.VertexBuffer;
		VertexBufferMemory = rhs.VertexBufferMemory;

		return*this;
	}

	VkBuffer GetVertexBuffer() { return VertexBuffer; }
};