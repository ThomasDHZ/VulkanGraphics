#pragma once
#include "GraphicsPipeline.h"
class MainPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass);
public:
	MainPipeline();
	MainPipeline(VkExtent2D swapChainExtent, VkRenderPass renderPass, VulkanDevice deviceInfo);
	~MainPipeline();
};

