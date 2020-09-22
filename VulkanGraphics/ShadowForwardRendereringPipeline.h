#pragma once
#include "GraphicsPipeline.h"

class ShadowForwardRendereringPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	ShadowForwardRendereringPipeline();
	ShadowForwardRendereringPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~ShadowForwardRendereringPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};

