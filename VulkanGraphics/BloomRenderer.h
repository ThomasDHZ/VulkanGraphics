#pragma once
#include "RendererBase.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"
#include "Rendering2DPipeline.h"
#include "Reflection2DPipeline.h"
#include "Rendering2DPipeline2.h"
#include "TextureManager.h"

class BloomRenderer : public RendererBase
{
private:
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer, std::shared_ptr<RendererColorTexture> texture);

public:
	BloomRenderer();
	BloomRenderer(VulkanRenderer& renderer);
	BloomRenderer(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture>& texture);
	~BloomRenderer();

	std::shared_ptr<Rendering2DPipeline2> renderer2DPipeline2;

	std::shared_ptr<RendererColorTexture> ColorTexture;
	std::shared_ptr<RendererColorTexture> BloomTexture;
	RendererDepthTexture DepthTexture;

	void UpdateSwapChain(VulkanRenderer& renderer);
	void UpdateSwapChain(VulkanRenderer& renderer, std::shared_ptr<RendererColorTexture> texture);
	void Destroy(VulkanRenderer& renderer) override;
};