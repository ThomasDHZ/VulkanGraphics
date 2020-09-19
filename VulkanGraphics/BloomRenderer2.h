#pragma once
#include "RendererBase.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"
#include "Rendering2DPipeline.h"
#include "Reflection2DPipeline.h"
#include "Rendering2DPipeline2.h"
#include "TextureManager.h"
#include "BloomPipeline.h"

class BloomRenderer2 : public RendererBase
{
private:
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer, std::shared_ptr<RendererColorTexture> texture);

public:
	BloomRenderer2();
	BloomRenderer2(VulkanRenderer& renderer);
	~BloomRenderer2();

	std::shared_ptr<BloomPipeline> bloomPipeline;

	std::shared_ptr<RendererColorTexture> BloomTexture;
	RendererDepthTexture DepthTexture;

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer) override;
};