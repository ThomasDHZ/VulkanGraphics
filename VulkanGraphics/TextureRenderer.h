#pragma once
#include "RendererBase.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"

class TextureRenderer : public RendererBase
{
private:
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);

public:
	TextureRenderer();
	TextureRenderer(VulkanRenderer& renderer);
	~TextureRenderer();

	ForwardRenderingPipeline forwardRendereringPipeline;
	SkyBoxPipeline skyboxPipeline;
	//FrameBufferRenderingPipeline frameBufferPipeline;
	DebugLightRenderingPipeline DebugLightPipeline;
	//CollisionDebugPipeline DebugCollisionPipeline;
	WireFramePipeline wireFramePipeline;

	RendererColorTexture ColorTexture;
	RendererDepthTexture DepthTexture;

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer);
};

