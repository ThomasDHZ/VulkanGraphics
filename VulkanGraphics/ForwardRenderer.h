#pragma once
#include "RendererDepthTexture.h"
#include "RendererBase2.h"

class ForwardRenderer : public RendererBase2
{
private:


public:
	ForwardRenderer();
	ForwardRenderer(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent, std::vector<VkImageView> swapChainImageViews);
	~ForwardRenderer();

	RendererDepthTexture DepthTexture;

	void CreateRenderPass(VkDevice Device);
	void CreateDescriptorSets(VkDevice Device);
	void CreateRenderingPipeline(VkDevice Device, VkExtent2D swapChainExtent);
	void CreateRendererFramebuffers(VkDevice Device, VkExtent2D swapChainExtent, std::vector<VkImageView> swapChainImageViews);
};

