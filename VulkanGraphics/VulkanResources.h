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
	static VkDescriptorSetLayout* GetMeshViewDescriptorSetLayout(VulkanRenderer& renderer);
	static VkPipeline* GetMeshViewShaderPipeline(VulkanRenderer& renderer);
	static VkPipelineLayout* GetMeshViewShaderPipelineLayout(VulkanRenderer& renderer);
	static VkDescriptorSetLayout* GetSkyboxDescriptorSetLayout(VulkanRenderer& renderer);
	static VkPipeline* GetSkyboxShaderPipeline(VulkanRenderer& renderer);
	static VkPipelineLayout* GetSkyboxShaderPipelineLayout(VulkanRenderer& renderer);
	static VkExtent2D* GetSwapChainResolution(VulkanRenderer& renderer);
	static uint32_t GetSwapChainMinImageCount(VulkanRenderer& renderer);
	static uint32_t GetSwapChainImageCount(VulkanRenderer& renderer);
};

