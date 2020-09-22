#pragma once
#include "GraphicsPipeline.h"

class Reflection2DPipeline :public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	Reflection2DPipeline();
	Reflection2DPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~Reflection2DPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};

