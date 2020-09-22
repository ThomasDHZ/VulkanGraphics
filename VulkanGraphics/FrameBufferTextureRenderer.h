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
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);

public:
	FrameBufferTextureRenderer();
	FrameBufferTextureRenderer(VulkanRenderer& renderer);
	~FrameBufferTextureRenderer();

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer) override;

	std::shared_ptr<BloomPipeline> bloomPipeline;
	std::shared_ptr<BloomPipeline2nd> bloomPipeline2nd;
	std::shared_ptr<FrameBufferRenderingPipeline> frameBufferPipeline;
	std::shared_ptr<Rendering2DPipeline> renderer2DPipeline;
	std::shared_ptr<UnderWater2DPipeline> underwater2DPipeline;

	std::shared_ptr<RendererColorTexture> ColorTexture;
	std::shared_ptr<RendererDepthTexture> DepthTexture;
};

