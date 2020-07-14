#pragma once
#include "GraphicsPipeline.h"

class RenderToTexturePipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
public:
	RenderToTexturePipeline();
	RenderToTexturePipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
	~RenderToTexturePipeline();

	void UpdateGraphicsPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
};

