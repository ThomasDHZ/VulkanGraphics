#pragma once
#include "RendererBase.h"
#include "RendererDepthTexture.h"
#include "ShadowForwardRendereringPipeline.h"

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

	std::shared_ptr<ShadowForwardRendereringPipeline> forwardRendereringPipeline;
	std::shared_ptr<RendererDepthTexture> DepthTexture;
};