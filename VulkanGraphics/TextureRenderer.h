#pragma once
#include "RendererBase2.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"

class TextureRenderer : public RendererBase2
{
private:

public:
	TextureRenderer();
	TextureRenderer(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent, std::vector<VkImageView> swapChainImageViews);
	~TextureRenderer();

	RendererColorTexture ColorTexture;
	RendererDepthTexture DepthTexture;

	void CreateRenderPass(VkDevice Device);
	void CreateDescriptorSets(VkDevice Device);
	void CreateRenderingPipeline(VkDevice Device, VkExtent2D swapChainExtent);
	void CreateRendererFramebuffers(VkDevice Device, VkExtent2D swapChainExtent, std::vector<VkImageView> swapChainImageViews);
};

