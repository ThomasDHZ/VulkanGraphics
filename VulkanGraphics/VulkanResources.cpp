#include "VulkanResources.h"

VkInstance* VulkanResources::GetInstance(VulkanRenderer& renderer)
{
	return &renderer.Instance;
}

VkDevice* VulkanResources::GetDevice(VulkanRenderer& renderer)
{
	return &renderer.Device;
}

VkPhysicalDevice* VulkanResources::GetPhysicalDevice(VulkanRenderer& renderer)
{
	return  &renderer.PhysicalDevice;
}

VkSurfaceKHR* VulkanResources::GetSurface(VulkanRenderer& renderer)
{
	return  &renderer.Surface;
}

VkQueue* VulkanResources::GetGraphicsQueue(VulkanRenderer& renderer)
{
	return  &renderer.GraphicsQueue;
}

VkQueue* VulkanResources::GetPresentQueue(VulkanRenderer& renderer)
{
	return  &renderer.PresentQueue;
}

VkRenderPass* VulkanResources::GetRenderPass(VulkanRenderer& renderer)
{
	return  &renderer.RenderPass;
}

VkDescriptorSetLayout* VulkanResources::GetDescriptorSetLayout(VulkanRenderer& renderer)
{
	return  &renderer.GraphicsPipeline.ShaderPipelineDescriptorLayout;
}

VkCommandPool* VulkanResources::GetCommandPool(VulkanRenderer& renderer)
{
	return &renderer.MainCommandPool;
}

VkPipeline* VulkanResources::GetShaderPipeline(VulkanRenderer& renderer)
{
	return &renderer.GraphicsPipeline.ShaderPipeline;
}

VkPipelineLayout* VulkanResources::GetShaderPipelineLayout(VulkanRenderer& renderer)
{
	return &renderer.GraphicsPipeline.ShaderPipelineLayout;
}

VkExtent2D* VulkanResources::GetSwapChainResolution(VulkanRenderer& renderer)
{
	return  &renderer.swapChain.GetSwapChainResolution();
}

uint32_t VulkanResources::GetSwapChainImageCount(VulkanRenderer& renderer)
{
	return  renderer.swapChain.GetSwapChainImageCount();
}
