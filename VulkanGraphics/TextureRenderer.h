#pragma once
#include "RendererBase.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"
#include "Rendering2DPipeline.h"
#include "Reflection2DPipeline.h"

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
	DebugLightRenderingPipeline DebugLightPipeline;
	Rendering2DPipeline renderer2DPipeline;
	//Reflection2DPipeline reflection2DPipeline;
	//CollisionDebugPipeline DebugCollisionPipeline;
	WireFramePipeline wireFramePipeline;

	RendererColorTexture ColorTexture;
	RendererDepthTexture DepthTexture;

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer) override;
};

