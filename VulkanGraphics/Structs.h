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

struct DescriptorSetLayoutBindingInfo
{
	uint32_t Binding;
	VkDescriptorType DescriptorType;
	VkShaderStageFlags StageFlags;
};

struct DescriptorPoolSizeInfo
{
	VkDescriptorType DescriptorType;
};

struct WriteDescriptorSetInfo
{
	uint32_t DstBinding;
	VkDescriptorSet DstSet;
	VkDescriptorType DescriptorType;
	VkDescriptorBufferInfo DescriptorBufferInfo;
	VkDescriptorImageInfo DescriptorImageInfo;
};