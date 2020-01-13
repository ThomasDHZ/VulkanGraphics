#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>

static class VulkanBufferManager
{
public:
static	VkCommandBuffer beginSingleTimeCommands(VkDevice Device, VkCommandPool& CommandPool)
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

static void endSingleTimeCommands(VkDevice Device, VkCommandBuffer commandBuffer, VkCommandPool& CommandPool, VkQueue& GraphicsQueue)
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

static void CopyBuffer(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool& CommandPool, VkQueue& GraphicsQueue)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(Device, CommandPool);

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(Device, commandBuffer, CommandPool, GraphicsQueue);
	}

static uint32_t FindMemoryType(VkPhysicalDevice PhysicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &memProperties);

		for (uint32_t x = 0; x < memProperties.memoryTypeCount; x++)
		{
			if ((typeFilter & (1 << x)) && (memProperties.memoryTypes[x].propertyFlags & properties) == properties) {
				return x;
			}
		}

		throw std::runtime_error("Failed to find suitable memory type.");
	}
};

