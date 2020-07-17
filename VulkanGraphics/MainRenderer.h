#pragma once
#include "VulkanRenderer.h"
#include "InputAttachment.h"
#include "ForwardRenderingPipeline.h"
class MainRenderer
{
private:

	void InitializeRenderPass(VulkanRenderer& renderer);

public:
	InputAttachment HDRColorAttachment;
	InputAttachment DepthAttachment;

	VkRenderPass RenderPass = VK_NULL_HANDLE;
	std::vector<VkFramebuffer> SwapChainFramebuffers;
	ForwardRenderingPipeline GraphicsPipeline;

	MainRenderer();
	MainRenderer(VulkanRenderer& renderer);
	~MainRenderer();

	void InitializeFramebuffers(VulkanRenderer& renderer);
};

