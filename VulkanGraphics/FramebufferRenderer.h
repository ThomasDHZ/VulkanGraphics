#pragma once

#include "RendererDepthTexture.h"
#include "RendererBase.h"
#include "BloomPipeline.h"

class FramebufferRenderer : public RendererBase
{
private:
	void SetUpColorBlendingSettings() override;
	void CreateRenderPass(VulkanEngine& renderer);
	void CreateRendererFramebuffers(VulkanEngine& renderer);

public:
	FramebufferRenderer();
	FramebufferRenderer(VulkanEngine& renderer);
	~FramebufferRenderer();

	void UpdateSwapChain(VulkanEngine& renderer);
	void Destroy(VulkanEngine& renderer) override;

	std::shared_ptr<FrameBufferRenderingPipeline> frameBufferPipeline;
	RendererDepthTexture DepthTexture;
};

