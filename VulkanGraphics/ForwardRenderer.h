#pragma once
#include "RendererDepthTexture.h"
#include "RendererHDRColorTexture.h"
#include "RendererBase.h"

class ForwardRenderer : public RendererBase
{
private:
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);

public:
	ForwardRenderer();
	ForwardRenderer(VulkanRenderer& renderer);
	~ForwardRenderer();

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer);

	RendererDepthTexture DepthTexture;
	RendererHDRColorTexture HDRColorTexture;
};

