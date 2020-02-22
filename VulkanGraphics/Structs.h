#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct VulkanDevice
{
	VkDevice Device;
	VkPhysicalDevice PhysicalDevice;
	VkCommandPool CommandPool;
	VkQueue GraphicsQueue;
	int SwapChainSize;
};