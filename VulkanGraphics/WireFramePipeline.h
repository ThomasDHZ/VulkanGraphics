#pragma once
#include "GraphicsPipeline.h"

class WireFramePipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
public:
	WireFramePipeline();
	WireFramePipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
	~WireFramePipeline();

	void UpdateGraphicsPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
};

