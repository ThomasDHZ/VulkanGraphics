#pragma once
#include "RendererDepthTexture.h"
#include "RendererHDRColorTexture.h"
#include "RendererBase.h"
#include "Rendering2DPipeline.h"

class ForwardRenderer : public RendererBase
{
private:
	void SetUpColorBlendingSettings() override;
	void CreateRenderPass(VulkanEngine& renderer);
	void CreateRendererFramebuffers(VulkanEngine& renderer);

public:
	ForwardRenderer();
	ForwardRenderer(VulkanEngine& renderer);
	~ForwardRenderer();

	void UpdateSwapChain(VulkanEngine& renderer);
	void Destroy(VulkanEngine& renderer) override;

	std::shared_ptr<FrameBufferRenderingPipeline> frameBufferPipeline;
	std::shared_ptr<ForwardRenderingPipeline> forwardRendereringPipeline;
	std::shared_ptr<Reflection2DPipeline> reflection2DPipeline;
	std::shared_ptr<Rendering2DPipeline> renderer2DPipeline;
	std::shared_ptr<UnderWater2DPipeline> underwater2DPipeline;
	//ReflectRendering2DPipeline reflectRenderer2DPipeline;
	std::shared_ptr<SkyBoxPipeline> skyboxPipeline;
	std::shared_ptr<DebugLightRenderingPipeline> DebugLightPipeline;
	std::shared_ptr<CollisionDebugPipeline> collisionDebugPipeline;
	std::shared_ptr<WireFramePipeline> wireFramePipeline;

	RendererDepthTexture DepthTexture;
};

