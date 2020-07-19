#pragma once
#include "RendererDepthTexture.h"
#include "RendererBase.h"

class ForwardRenderer : public RendererBase
{
private:
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateDescriptorSets(VulkanRenderer& renderer);
	void CreateRenderingPipeline(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);

public:
	ForwardRenderer();
	ForwardRenderer(VulkanRenderer& renderer);
	~ForwardRenderer();

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer);

	RendererDepthTexture DepthTexture;
};

