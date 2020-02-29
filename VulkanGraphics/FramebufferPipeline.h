#pragma once
#include "GraphicsPipeline.h"

class FramebufferPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass);
public:
	FramebufferPipeline();
	FramebufferPipeline(VkExtent2D swapChainExtent, VkRenderPass renderPass, VulkanDevice deviceInfo);
	~FramebufferPipeline();
};

