#pragma once

#include "RendererDepthTexture.h"
#include "RendererHDRColorTexture.h"
#include "RendererBase.h"

class FramebufferRenderer : public RendererBase
{
private:
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);

public:
	FramebufferRenderer();
	FramebufferRenderer(VulkanRenderer& renderer);
	~FramebufferRenderer();

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer) override;

	std::shared_ptr<FrameBufferRenderingPipeline> frameBufferPipeline;
	RendererDepthTexture DepthTexture;
};

