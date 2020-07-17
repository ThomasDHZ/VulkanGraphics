#pragma once
#include "RendererBase2.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"

class TextureSkyboxRenderer : public RendererBase2
{
private:

public:
	TextureSkyboxRenderer();
	TextureSkyboxRenderer(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent, std::vector<VkImageView> swapChainImageViews);
	~TextureSkyboxRenderer();

	RendererColorTexture ColorTexture;
	RendererDepthTexture DepthTexture;

	void CreateRenderPass(VkDevice Device);
	void CreateDescriptorSets(VkDevice Device);
	void CreateRenderingPipeline(VkDevice Device, VkExtent2D swapChainExtent);
	void CreateRendererFramebuffers(VkDevice Device, VkExtent2D swapChainExtent, std::vector<VkImageView> swapChainImageViews);
};

