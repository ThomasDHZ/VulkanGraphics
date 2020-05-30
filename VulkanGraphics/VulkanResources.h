#pragma once
#include "VulkanRenderer.h"

class VulkanResources
{
public:
	static VkInstance* GetInstance(Renderer& renderer);
	static VkDevice* GetDevice(Renderer& renderer);
	static VkPhysicalDevice* GetPhysicalDevice(Renderer& renderer);
	static VkSurfaceKHR* GetSurface(Renderer& renderer);
	static VkQueue* GetGraphicsQueue(Renderer& renderer);
	static VkQueue* GetPresentQueue(Renderer& renderer);
	static VkRenderPass* GetRenderPass(Renderer& renderer);
	static VkDescriptorSetLayout* GetDescriptorSetLayout(Renderer& renderer);
	static VkCommandPool* GetSecondaryCommandPool(Renderer& renderer);
	static VkCommandBuffer* GetSecondaryCommandBuffer(Renderer& renderer, unsigned int frame);
	static VkPipeline* GetShaderPipeline(Renderer& renderer);
	static VkPipelineLayout* GetShaderPipelineLayout(Renderer& renderer);
	static VkDescriptorSetLayout* GetMeshViewDescriptorSetLayout(Renderer& renderer);
	static DebugLightRenderingPipeline GetDebugLightPipeline(Renderer& renderer);
	static VkPipeline* GetMeshViewShaderPipeline(Renderer& renderer);
	static VkPipelineLayout* GetMeshViewShaderPipelineLayout(Renderer& renderer);
	static VkDescriptorSetLayout* GetSkyboxDescriptorSetLayout(Renderer& renderer);
	static VkPipeline* GetSkyboxShaderPipeline(Renderer& renderer);
	static VkPipelineLayout* GetSkyboxShaderPipelineLayout(Renderer& renderer);
	static VkExtent2D* GetSwapChainResolution(Renderer& renderer);
	static uint32_t GetSwapChainMinImageCount(Renderer& renderer);
	static uint32_t GetSwapChainImageCount(Renderer& renderer);
};

