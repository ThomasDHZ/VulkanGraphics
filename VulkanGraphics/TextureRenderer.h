#pragma once
#include "RendererBase.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"

class TextureRenderer : public RendererBase
{
private:

public:
	TextureRenderer();
	TextureRenderer(VulkanRenderer& renderer);
	~TextureRenderer();

	RendererColorTexture ColorTexture;
	RendererDepthTexture DepthTexture;

	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateDescriptorSets(VulkanRenderer& renderer);
	void CreateRenderingPipeline(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer);
};

