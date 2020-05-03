#pragma once
#include "GraphicsPipeline.h"

class SkyBoxPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDevice device);
public:
	SkyBoxPipeline();
	SkyBoxPipeline(VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDevice device);
	~SkyBoxPipeline();

	void UpdateGraphicsPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDevice device);
};