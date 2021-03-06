#pragma once
#include "RendererBase.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"
#include "Rendering2DPipeline.h"
#include "Reflection2DPipeline.h"
#include "BloomPipeline.h"

class TextureRenderer : public RendererBase
{
private:
	void SetUpColorBlendingSettings() override;
	void CreateRenderPass(VulkanEngine& renderer);
	void CreateRendererFramebuffers(VulkanEngine& renderer);

public:
	TextureRenderer();
	TextureRenderer(VulkanEngine& renderer);
	~TextureRenderer();

	std::shared_ptr<ForwardRenderingPipeline> forwardRendereringPipeline;
	std::shared_ptr<SkyBoxPipeline> skyboxPipeline;
	std::shared_ptr<DebugLightRenderingPipeline> DebugLightPipeline;
	std::shared_ptr<Rendering2DPipeline> renderer2DPipeline;
	//Reflection2DPipeline reflection2DPipeline;
	//CollisionDebugPipeline DebugCollisionPipeline;
	std::shared_ptr<WireFramePipeline> wireFramePipeline;
	std::shared_ptr<BloomPipeline> bloomPipeline;

	std::shared_ptr<RendererColorTexture> ColorTexture;
	RendererDepthTexture DepthTexture;

	void UpdateSwapChain(VulkanEngine& renderer);
	void Destroy(VulkanEngine& renderer) override;
};

