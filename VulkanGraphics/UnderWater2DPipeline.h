#pragma once
#include "GraphicsPipeline.h"

class UnderWater2DPipeline :public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	UnderWater2DPipeline();
	UnderWater2DPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~UnderWater2DPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};