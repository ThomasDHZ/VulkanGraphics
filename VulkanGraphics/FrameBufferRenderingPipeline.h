#pragma once
#include "GraphicsPipeline.h"

class FrameBufferRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
public:
	FrameBufferRenderingPipeline();
	FrameBufferRenderingPipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
	~FrameBufferRenderingPipeline();

	void UpdateGraphicsPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
};

