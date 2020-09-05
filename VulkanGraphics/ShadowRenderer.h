#pragma once
#include "RendererBase.h"
#include "RendererDepthTexture.h"

class ShadowRenderer : public RendererBase
{
private:
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);

public:
	ShadowRenderer();
	ShadowRenderer(VulkanRenderer& renderer);
	~ShadowRenderer();

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer) override;

	ForwardRenderingPipeline forwardRendereringPipeline;
	RendererDepthTexture DepthTexture;
};