#pragma once
#include <vulkan\vulkan_core.h>

struct VulkanFrame
{

};

struct VulkanDevice32
{
	VulkanWindow Window;
	VkInstance Instance;

	VkDevice Device;
	VkPhysicalDevice PhysicalDevice;
	VkCommandPool CommandPool;
	VkQueue GraphicsQueue;
	VkDescriptorSetLayout descriptorSetLayout;
	int SwapChainSize;
} vulkanDevice2;
