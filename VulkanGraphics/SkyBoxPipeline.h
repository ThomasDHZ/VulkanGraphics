#pragma once
#include "GraphicsPipeline.h"
class SkyBoxPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass);
public:
	SkyBoxPipeline();
	SkyBoxPipeline(VkExtent2D swapChainExtent, VkRenderPass renderPass, VulkanDevice deviceInfo);
	~SkyBoxPipeline();
};

