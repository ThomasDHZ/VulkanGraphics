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

VkDescriptorSetLayout* VulkanResources::GetMeshViewDescriptorSetLayout(VulkanRenderer& renderer)
{
	return &renderer.MeshviewPipeline.ShaderPipelineDescriptorLayout;
}

VkPipeline* VulkanResources::GetMeshViewShaderPipeline(VulkanRenderer& renderer)
{
	return &renderer.MeshviewPipeline.ShaderPipeline;
}

VkPipelineLayout* VulkanResources::GetMeshViewShaderPipelineLayout(VulkanRenderer& renderer)
{
	return &renderer.MeshviewPipeline.ShaderPipelineLayout;
}

VkDescriptorSetLayout* VulkanResources::GetSkyboxDescriptorSetLayout(VulkanRenderer& renderer)
{
	return &renderer.SkyboxPipeline.ShaderPipelineDescriptorLayout;
}

VkPipeline* VulkanResources::GetSkyboxShaderPipeline(VulkanRenderer& renderer)
{
	return &renderer.SkyboxPipeline.ShaderPipeline;
}

VkPipelineLayout* VulkanResources::GetSkyboxShaderPipelineLayout(VulkanRenderer& renderer)
{
	return &renderer.SkyboxPipeline.ShaderPipelineLayout;
}

VkExtent2D* VulkanResources::GetSwapChainResolution(VulkanRenderer& renderer)
{
	return  &renderer.swapChain.GetSwapChainResolution();
}

uint32_t VulkanResources::GetSwapChainMinImageCount(VulkanRenderer& renderer)
{
	return renderer.swapChain.GetSwapChainMinImageCount();
}

uint32_t VulkanResources::GetSwapChainImageCount(VulkanRenderer& renderer)
{
	return  renderer.swapChain.GetSwapChainImageCount();
}
