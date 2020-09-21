#pragma once
#include "GraphicsPipeline.h"

class ShadowForwardRendereringPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	ShadowForwardRendereringPipeline();
	ShadowForwardRendereringPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~ShadowForwardRendereringPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

