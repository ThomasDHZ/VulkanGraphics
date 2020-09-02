#pragma once
#include "RendererDepthTexture.h"
#include "RendererHDRColorTexture.h"
#include "RendererBase.h"
#include "Rendering2DPipeline.h"

class ForwardRenderer : public RendererBase
{
private:
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);

public:
	ForwardRenderer();
	ForwardRenderer(VulkanRenderer& renderer);
	~ForwardRenderer();

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer);

	ForwardRenderingPipeline forwardRendereringPipeline;
	Rendering2DPipeline renderer2DPipeline;
	//ReflectRendering2DPipeline reflectRenderer2DPipeline;
	SkyBoxPipeline skyboxPipeline;
	DebugLightRenderingPipeline DebugLightPipeline;
	//CollisionDebugPipeline DebugCollisionPipeline;
	WireFramePipeline wireFramePipeline;

	RendererDepthTexture DepthTexture;
	RendererHDRColorTexture HDRColorTexture;
};

