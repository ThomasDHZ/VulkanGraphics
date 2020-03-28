#pragma once
#include "GraphicsPipeline.h"

class ForwardRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D& swapChainExtent, VkRenderPass& renderPass);
public:
	ForwardRenderingPipeline();
	ForwardRenderingPipeline(VkExtent2D& swapChainExtent, VkRenderPass& renderPass, VulkanDevice deviceInfo);
	~ForwardRenderingPipeline();

	void RecreatePipeline(VkExtent2D& swapChainExtent, VkRenderPass& renderPass);
};

