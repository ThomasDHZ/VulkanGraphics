#pragma once
#include "RendererBase.h"
#include "RendererDepthTexture.h"
#include "ShadowForwardRendereringPipeline.h"

class ShadowRenderer : public RendererBase
{
private:
	void SetUpColorBlendingSettings() override;
	void CreateRenderPass(VulkanEngine& renderer);
	void CreateRendererFramebuffers(VulkanEngine& renderer);

public:
	ShadowRenderer();
	ShadowRenderer(VulkanEngine& renderer);
	~ShadowRenderer();

	void UpdateSwapChain(VulkanEngine& renderer);
	void Destroy(VulkanEngine& renderer) override;

	std::shared_ptr<ShadowForwardRendereringPipeline> forwardRendereringPipeline;
	std::shared_ptr<Rendering2DPipeline> renderer2DPipeline;
	std::shared_ptr<RendererDepthTexture> DepthTexture;
};