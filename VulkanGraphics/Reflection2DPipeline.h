#pragma once
#include "GraphicsPipeline.h"

class Reflection2DPipeline :public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	Reflection2DPipeline();
	Reflection2DPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~Reflection2DPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

