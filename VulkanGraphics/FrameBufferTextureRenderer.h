#pragma once
#include "RendererBase.h"
#include "RendererDepthTexture.h"
#include "RendererColorTexture.h"
#include <vulkan/vulkan_core.h>
#include "BloomPipeline.h"
#include "BloomPipeline2nd.h"
class FrameBufferTextureRenderer : public RendererBase
{
private:
	void SetUpColorBlendingSettings() override;
	void CreateRenderPass(VulkanEngine& renderer);
	void CreateRendererFramebuffers(VulkanEngine& renderer);

public:
	FrameBufferTextureRenderer();
	FrameBufferTextureRenderer(VulkanEngine& renderer);
	~FrameBufferTextureRenderer();

	void UpdateSwapChain(VulkanEngine& renderer);
	void Destroy(VulkanEngine& renderer) override;

	std::shared_ptr<BloomPipeline> bloomPipeline;
	std::shared_ptr<BloomPipeline2nd> bloomPipeline2nd;
	std::shared_ptr<FrameBufferRenderingPipeline> frameBufferPipeline;
	std::shared_ptr<Rendering2DPipeline> renderer2DPipeline;

	std::shared_ptr<RendererColorTexture> ColorTexture;
	std::shared_ptr<RendererDepthTexture> DepthTexture;
};

