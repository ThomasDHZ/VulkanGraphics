#pragma once
#include "VulkanRenderer.h"

class VulkanResources
{
public:
	static VkInstance* GetInstance(VulkanRenderer& renderer);
	static VkDevice* GetDevice(VulkanRenderer& renderer);
	static VkPhysicalDevice* GetPhysicalDevice(VulkanRenderer& renderer);
	static VkSurfaceKHR* GetSurface(VulkanRenderer& renderer);
	static VkQueue* GetGraphicsQueue(VulkanRenderer& renderer);
	static VkQueue* GetPresentQueue(VulkanRenderer& renderer);
	static VkRenderPass* GetRenderPass(VulkanRenderer& renderer);
	static VkDescriptorSetLayout* GetDescriptorSetLayout(VulkanRenderer& renderer);
	static VkCommandPool* GetCommandPool(VulkanRenderer& renderer);
	static VkPipeline* GetShaderPipeline(VulkanRenderer& renderer);
	static VkPipelineLayout* GetShaderPipelineLayout(VulkanRenderer& renderer);
	static VkExtent2D* GetSwapChainResolution(VulkanRenderer& renderer);
	static uint32_t GetSwapChainImageCount(VulkanRenderer& renderer);
};

