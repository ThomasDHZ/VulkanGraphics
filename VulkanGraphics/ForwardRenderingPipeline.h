#pragma once
#include "GraphicsPipeline.h"

class ForwardRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
public:
	ForwardRenderingPipeline();
	ForwardRenderingPipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
	~ForwardRenderingPipeline();

	void UpdateGraphicsPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
};

