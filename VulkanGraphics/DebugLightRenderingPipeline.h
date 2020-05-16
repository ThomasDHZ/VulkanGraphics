#pragma once
#include "GraphicsPipeline.h"
class DebugLightRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
public:
	DebugLightRenderingPipeline();
	DebugLightRenderingPipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
	~DebugLightRenderingPipeline();

	void UpdateGraphicsPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
};
