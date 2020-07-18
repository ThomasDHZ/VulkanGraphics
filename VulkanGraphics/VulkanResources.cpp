#include "VulkanResources.h"

VkInstance* VulkanResources::GetInstance(Renderer& renderer)
{
	return &renderer.Instance;
}

VkDevice* VulkanResources::GetDevice(Renderer& renderer)
{
	return &renderer.Device;
}

VkPhysicalDevice* VulkanResources::GetPhysicalDevice(Renderer& renderer)
{
	return  &renderer.PhysicalDevice;
}

VkSurfaceKHR* VulkanResources::GetSurface(Renderer& renderer)
{
	return  &renderer.Surface;
}

VkQueue* VulkanResources::GetGraphicsQueue(Renderer& renderer)
{
	return  &renderer.GraphicsQueue;
}

VkQueue* VulkanResources::GetPresentQueue(Renderer& renderer)
{
	return  &renderer.PresentQueue;
}

VkRenderPass* VulkanResources::GetRenderPass(Renderer& renderer)
{
	return  &renderer.forwardRenderer.RenderPass;
}

VkDescriptorSetLayout* VulkanResources::GetDescriptorSetLayout(Renderer& renderer)
{
	return  &renderer.forwardRenderer.DescriptorSetLayout;
}

VkCommandPool* VulkanResources::GetRendererCommandPool(Renderer& renderer)
{
	return &renderer.RenderCommandPool;
}

VkCommandBuffer* VulkanResources::GetRendererCommandBuffer(Renderer& renderer, unsigned int frame)
{
	return &renderer.RenderCommandBuffer[frame];
}

VkPipeline* VulkanResources::GetShaderPipeline(Renderer& renderer)
{
	return &renderer.forwardRenderer.RendererPipeline;
}

VkPipelineLayout* VulkanResources::GetShaderPipelineLayout(Renderer& renderer)
{
	return &renderer.forwardRenderer.RendererLayout;
}

VkDescriptorSetLayout* VulkanResources::GetMeshViewDescriptorSetLayout(Renderer& renderer)
{
	return &renderer.MeshviewPipeline.ShaderPipelineDescriptorLayout;
}

DebugLightRenderingPipeline VulkanResources::GetDebugLightPipeline(Renderer& renderer)
{
	return renderer.DebugLightPipeline;
}

VkPipeline* VulkanResources::GetMeshViewShaderPipeline(Renderer& renderer)
{
	return &renderer.MeshviewPipeline.ShaderPipeline;
}

VkPipelineLayout* VulkanResources::GetMeshViewShaderPipelineLayout(Renderer& renderer)
{
	return &renderer.MeshviewPipeline.ShaderPipelineLayout;
}

VkPipeline* VulkanResources::GetCollisionShaderPipeline(Renderer& renderer)
{
	return &renderer.DebugCollisionPipeline.ShaderPipeline;
}

VkPipelineLayout* VulkanResources::GetCollisionShaderPipelineLayout(Renderer& renderer)
{
	return &renderer.DebugCollisionPipeline.ShaderPipelineLayout;
}

VkDescriptorSetLayout* VulkanResources::GetSkyboxDescriptorSetLayout(Renderer& renderer)
{
	return &renderer.SkyboxPipeline.ShaderPipelineDescriptorLayout;
}

VkPipeline* VulkanResources::GetSkyboxShaderPipeline(Renderer& renderer)
{
	return &renderer.SkyboxPipeline.ShaderPipeline;
}

VkPipelineLayout* VulkanResources::GetSkyboxShaderPipelineLayout(Renderer& renderer)
{
	return &renderer.SkyboxPipeline.ShaderPipelineLayout;
}

VkExtent2D* VulkanResources::GetSwapChainResolution(Renderer& renderer)
{
	return  &renderer.SwapChain.GetSwapChainResolution();
}

uint32_t VulkanResources::GetSwapChainMinImageCount(Renderer& renderer)
{
	return renderer.SwapChain.GetSwapChainMinImageCount();
}

uint32_t VulkanResources::GetSwapChainImageCount(Renderer& renderer)
{
	return  renderer.SwapChain.GetSwapChainImageCount();
}
