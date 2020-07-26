#pragma once
#include "GraphicsPipeline.h"

class ShadowRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	ShadowRenderingPipeline();
	ShadowRenderingPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~ShadowRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};


