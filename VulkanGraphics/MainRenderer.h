#pragma once
#include "RendererBase.h"
#include "InputAttachment.h"
#include "ForwardRenderingPipeline.h"
class MainRenderer
{
private:

	void InitializeRenderPass(RendererBase& renderer);

public:
	InputAttachment HDRColorAttachment;
	InputAttachment DepthAttachment;

	VkRenderPass RenderPass = VK_NULL_HANDLE;
	std::vector<VkFramebuffer> SwapChainFramebuffers;
	ForwardRenderingPipeline GraphicsPipeline;

	MainRenderer();
	MainRenderer(RendererBase& renderer);
	~MainRenderer();

	void InitializeFramebuffers(RendererBase& renderer);
};

