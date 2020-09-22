#pragma once
#include "RendererBase.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"
#include "Rendering2DPipeline.h"
#include "Reflection2DPipeline.h"
#include "TextureManager.h"

class SceneRenderer : public RendererBase
{
private:
    void SetUpColorBlendingSettings() override;
	void CreateRenderPass(VulkanEngine& renderer);
	void CreateRendererFramebuffers(VulkanEngine& renderer);

public:
	SceneRenderer();
	SceneRenderer(VulkanEngine& renderer);
	~SceneRenderer();

	std::shared_ptr<Rendering2DPipeline> renderer2DPipeline2;

	std::shared_ptr<RendererColorTexture> ColorTexture;
	std::shared_ptr<RendererColorTexture> BloomTexture;
	RendererDepthTexture DepthTexture;

	void UpdateSwapChain(VulkanEngine& renderer);
	void Destroy(VulkanEngine& renderer) override;
};