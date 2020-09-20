#pragma once
#include "RendererBase.h"
#include "RendererColorTexture.h"
#include "RendererDepthTexture.h"
#include "Rendering2DPipeline.h"
#include "Reflection2DPipeline.h"
#include "Rendering2DPipeline2.h"
#include "TextureManager.h"

class SceneRenderer : public RendererBase
{
private:
	void CreateRenderPass(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer);
	void CreateRendererFramebuffers(VulkanRenderer& renderer, std::shared_ptr<RendererColorTexture> texture);

public:
	SceneRenderer();
	SceneRenderer(VulkanRenderer& renderer);
	~SceneRenderer();

	std::shared_ptr<ForwardRenderingPipeline> forwardRendereringPipeline;
	std::shared_ptr<Reflection2DPipeline> reflection2DPipeline;
	std::shared_ptr<Rendering2DPipeline> renderer2DPipeline;
	std::shared_ptr<UnderWater2DPipeline> underwater2DPipeline;
	//ReflectRendering2DPipeline reflectRenderer2DPipeline;
	std::shared_ptr<SkyBoxPipeline> skyboxPipeline;
	std::shared_ptr<DebugLightRenderingPipeline> DebugLightPipeline;
	std::shared_ptr<CollisionDebugPipeline> collisionDebugPipeline;
	std::shared_ptr<WireFramePipeline> wireFramePipeline;

	std::shared_ptr<RendererColorTexture> ColorTexture;
	std::shared_ptr<RendererColorTexture> BloomTexture;
	RendererDepthTexture DepthTexture;

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer) override;
};

