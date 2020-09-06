#pragma once
#include "GraphicsPipeline.h"

class UnderWater2DPipeline :public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	UnderWater2DPipeline();
	UnderWater2DPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~UnderWater2DPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};