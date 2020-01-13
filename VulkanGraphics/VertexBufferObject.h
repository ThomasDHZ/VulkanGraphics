#pragma once
#include <vulkan/vulkan.h>
#include <vector>

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

		VkDeviceSize bufferSize = sizeof(VertexData[0]) * VertexData.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(Device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, VertexData.data(), (size_t)bufferSize);
		vkUnmapMemory(Device, stagingBufferMemory);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferMemory);

		CopyBuffer(stagingBuffer, VertexBuffer, bufferSize, CommandPool, GraphicsQueue);

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

		if (vkCreateBuffer(Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(Device, buffer, bufferMemory, 0);
	}

	VkCommandBuffer beginSingleTimeCommands(VkCommandPool& CommandPool)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = CommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(Device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool& CommandPool, VkQueue& GraphicsQueue)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(GraphicsQueue);

		vkFreeCommandBuffers(Device, CommandPool, 1, &commandBuffer);
	}

	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool& CommandPool, VkQueue& GraphicsQueue)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(CommandPool);

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer, CommandPool, GraphicsQueue);
	}

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &memProperties);

		for (uint32_t x = 0; x < memProperties.memoryTypeCount; x++)
		{
			if ((typeFilter & (1 << x)) && (memProperties.memoryTypes[x].propertyFlags & properties) == properties) {
				return x;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void CleanUp()
	{
		vkDestroyBuffer(Device, VertexBuffer, nullptr);
		vkFreeMemory(Device, VertexBufferMemory, nullptr);
	}

	VkBuffer GetVertexBuffer() { return VertexBuffer; }
	VkDeviceMemory GetVertexBufferMemory() { return VertexBufferMemory; }
};