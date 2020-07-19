#pragma once
#include "RendererDepthTexture.h"
#include "RendererBase.h"

class ForwardRenderer : public RendererBase
{
private:
	void CreateRenderPass(VkDevice Device);
	void CreateDescriptorSets(VkDevice Device);
	void CreateRenderingPipeline(VkDevice Device, VkExtent2D swapChainExtent);
	void CreateRendererFramebuffers(VkDevice Device, VkExtent2D swapChainExtent, std::vector<VkImageView> swapChainImageViews);

public:
	ForwardRenderer();
	ForwardRenderer(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent, std::vector<VkImageView> swapChainImageViews);
	~ForwardRenderer();

	void UpdateSwapChain(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent, std::vector<VkImageView> swapChainImageViews);
	void Destroy(VkDevice Device);

	RendererDepthTexture DepthTexture;
};

